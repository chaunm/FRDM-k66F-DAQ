/* Ftp.c 
* Process for updating firmware through FTP protocol 	
* author: ChauNM										
* Date: 09 Aug 2018										
*/
#include "net_config.h"
#include "core/net.h"
#include "ftp/ftp_client.h"
#include "ftp.h"
#include "debug.h"
// FreeRTOS inclusions
#include "os_port_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "internal_flash.h"
#include "hal_system.h"
#include "snmpConnect_manager.h"
#include "mqtt_client/mqtt_json_make.h"
#include "mqtt_client/app_mqtt_client.h"
#include "variables.h"

#include <stdlib.h>

#define FTP_USER		"DAQ_FIRMWARE"
#define FTP_PSW			"12345678"
#define FTP_SERVER_PORT 21
#define FIRMWARE_KEY_1	0x55AAAA55
#define FIRMWARE_KEY_2 	0x2112FEEF


uint8_t firmwareBuffer[FTP_FIRMWARE_BUFFER_SIZE];
uint8_t fwWriteBuffer[FTP_FIRMWARE_BUFFER_SIZE];
size_t writeBufferOffset;

FtpServerInfo_t firmwareInfo;
TaskHandle_t ftpFirmwareTask;

/**
* @brief FTP client test routine
* @return Error code
**/
void FTP_FirmwareUpdateTask(void* param)
{
	FtpServerInfo_t* serverInfo = (FtpServerInfo_t*)param;
	FtpClientContext ftpContext;
	IpAddr ipAddr;
    error_t error;
    size_t length;   
	size_t totalRead, totalWrite;
	bool imageGetStatus = false;
	uint32_t imageUpdateKey[2] = {FIRMWARE_KEY_1, FIRMWARE_KEY_2};
	uint32_t flashAddr = IMAGE_START_ADDR;
    size_t writeBufferFreeCount;
	FtpFwUpdateStatus_t status;
	char* reportMessage;
    //Debug message
    TRACE_INFO("\r\n\r\nResolving server name...\r\n");
    error = getHostByName(NULL, serverInfo->serverIp, &ipAddr, 0);
    if(error)
    {
        //Debug message
        TRACE_INFO("Failed to resolve server name!\r\n");
		reportMessage = mqtt_json_make_fw_update_result(deviceName, serverInfo->serverIp, serverInfo->fileName, 
														(int32_t)FW_UPDATE_STATUS_SERVER_CONNECT_ERROR);
		mqttPublishMsg(MQTT_EVENT_TOPIC, reportMessage, strlen(reportMessage));
		free(reportMessage);
		free(serverInfo->fileName);
		free(serverInfo->serverIp);
		serverInfo->fileName = NULL;
		serverInfo->serverIp = NULL;
		serverInfo->fileSize = 0;
        vTaskDelete(NULL);
    }
    
    //Debug message
    TRACE_INFO("Connecting to FTP server for firmware update%s\r\n", ipAddrToString(&ipAddr, NULL));
//    osDelayTask(10000);
	if (interfaceManagerGetActiveInterface() == NULL)
	{
		TRACE_INFO("Failed to resolve server name!\r\n");
		reportMessage = mqtt_json_make_fw_update_result(deviceName, serverInfo->serverIp, serverInfo->fileName, 
														(int32_t)FW_UPDATE_STATUS_NETWORK_ERROR);
		mqttPublishMsg(MQTT_EVENT_TOPIC, reportMessage, strlen(reportMessage));
		free(reportMessage);
		free(serverInfo->fileName);
		free(serverInfo->serverIp);
		serverInfo->fileName = NULL;
		serverInfo->serverIp = NULL;
		serverInfo->fileSize = 0;
        vTaskDelete(NULL);
	}
    //Connect to the FTP server using active interface
    error = ftpConnect(&ftpContext, interfaceManagerGetActiveInterface(), &ipAddr, FTP_SERVER_PORT, FTP_NO_SECURITY | FTP_PASSIVE_MODE);
    
    if(error)
    {
        //Debug message
        TRACE_INFO("Failed to connect to FTP server!\r\n");
		reportMessage = mqtt_json_make_fw_update_result(deviceName, serverInfo->serverIp, serverInfo->fileName, 
														(int32_t)FW_UPDATE_STATUS_SERVER_CONNECT_ERROR);
		mqttPublishMsg(MQTT_EVENT_TOPIC, reportMessage, strlen(reportMessage));
		free(reportMessage);
		free(serverInfo->fileName);
		free(serverInfo->serverIp);
		serverInfo->fileName = NULL;
		serverInfo->serverIp = NULL;
		serverInfo->fileSize = 0;
        //Exit immediately
        vTaskDelete(NULL);
    }
    
    //Debug message
    TRACE_INFO("connect to ftp server succeed\r\n");
	// Initialize flash before updating firmare
	IFLASH_Init();
	if (IFLASH_Erase(IMAGE_START_ADDR, IMAGE_SIZE) != kStatus_FLASH_Success)
	{
		//Debug message
        TRACE_INFO("Failed format flash!\r\n");
		reportMessage = mqtt_json_make_fw_update_result(deviceName, serverInfo->serverIp, serverInfo->fileName, 
														(int32_t)FW_UPDATE_STATUS_FLASH_ERROR);
		mqttPublishMsg(MQTT_EVENT_TOPIC, reportMessage, strlen(reportMessage));
		free(reportMessage);
		free(serverInfo->fileName);
		free(serverInfo->serverIp);
		serverInfo->fileName = NULL;
		serverInfo->serverIp = NULL;
		serverInfo->fileSize = 0;
        //Exit immediately
        vTaskDelete(NULL);
	}
	totalRead = 0;
    totalWrite = 0;
    writeBufferOffset = 0;
	status = FW_UPDATE_STATUS_SUCCESS;
    do
    {
        //Login to the FTP server using the provided username and password
        error = ftpLogin(&ftpContext, FTP_USER, FTP_PSW, "");
        if(error) 
		{
			status = FW_UPDATE_STATUS_LOGIN_ERROR;
			break;
		}
        //Open file
        error = ftpOpenFile(&ftpContext, serverInfo->fileName, FTP_FOR_READING | FTP_BINARY_TYPE);
        if(error) 
		{
			status = FW_UPDATE_STATUS_FILE_OPEN_ERROR;
            break;
		}
        
        while(1)
        {
            //Read data
            error = ftpReadFile(&ftpContext, firmwareBuffer, FTP_FIRMWARE_BUFFER_SIZE, &length, 0);
            //End of file?
            if(error) 
			{				
				break;
			}
            totalRead += length;
            TRACE_INFO("Read %d bytes, total %d bytes of %d bytes\r\n", length, totalRead, serverInfo->fileSize);
            // data alignment for flash to write is 8 bytes
            // this code for handle data alignment
            writeBufferFreeCount = FTP_FIRMWARE_BUFFER_SIZE - writeBufferOffset;
            if (length <= writeBufferFreeCount)
            {
                memcpy(fwWriteBuffer + writeBufferOffset, firmwareBuffer, length);
                writeBufferOffset += length;
                if (writeBufferOffset == FTP_FIRMWARE_BUFFER_SIZE)
                {
                    if (IFLASH_CopyRamToFlash(flashAddr, (uint32_t*)fwWriteBuffer, FTP_FIRMWARE_BUFFER_SIZE) != kStatus_FLASH_Success)
					{
						status = FW_UPDATE_STATUS_FLASH_ERROR;
						break;
					}
                    memset(fwWriteBuffer, 0xFF, sizeof(fwWriteBuffer));
                    totalWrite += FTP_FIRMWARE_BUFFER_SIZE;
                    writeBufferOffset  = 0;
                    TRACE_INFO("Write %d bytes of %d\r\n", totalWrite, serverInfo->fileSize);
                    flashAddr += FTP_FIRMWARE_BUFFER_SIZE;                  
                }                 
            }
            else
            {
                memcpy(fwWriteBuffer + writeBufferOffset, firmwareBuffer, writeBufferFreeCount);
                // write buffer is already full, write buffer to flash
                if (IFLASH_CopyRamToFlash(flashAddr, (uint32_t*)fwWriteBuffer, FTP_FIRMWARE_BUFFER_SIZE) != kStatus_FLASH_Success)
				{
					status = FW_UPDATE_STATUS_FLASH_ERROR;
					break;
				}
                memset(fwWriteBuffer, 0xFF, sizeof(fwWriteBuffer));                
                totalWrite += FTP_FIRMWARE_BUFFER_SIZE;
                TRACE_INFO("Write %d bytes of %d\r\n", totalWrite, serverInfo->fileSize);
                flashAddr += FTP_FIRMWARE_BUFFER_SIZE;
                writeBufferOffset  = 0;
                // copy remaining read data to write buffer
                memcpy(fwWriteBuffer, firmwareBuffer + writeBufferFreeCount, length - writeBufferFreeCount);
                writeBufferOffset = length - writeBufferFreeCount;                
            }
            // if read all data from server, write remaining data in writeBuffer to flash
            if (totalRead == serverInfo->fileSize)
            {                
                if (IFLASH_CopyRamToFlash(flashAddr, (uint32_t*)fwWriteBuffer, FTP_FIRMWARE_BUFFER_SIZE) != kStatus_FLASH_Success)
				{
					status = FW_UPDATE_STATUS_FLASH_ERROR;
					break;
				}
                totalWrite += writeBufferOffset;
                TRACE_INFO("Write %d bytes of %d\r\n", totalWrite, serverInfo->fileSize);        
            }
        }
        if (totalWrite != serverInfo->fileSize)
		{
			TRACE_INFO("Read file error \r\n");
			if (status == FW_UPDATE_STATUS_SUCCESS)
				status = FW_UPDATE_STATUS_FILE_SIZE_ERROR;
			// erase flash
			IFLASH_Erase(IMAGE_START_ADDR, IMAGE_SIZE);
			imageGetStatus = false;
		}
		else
		{
			// all image has receive - restart device to let bootloader update new image
			imageGetStatus = true;
		}
        //Close the file
        error = ftpCloseFile(&ftpContext);
        
        //End of exception handling block
    } while(0);
    
    //Close the connection
	TRACE_INFO("FTP Connection closed...\r\n");
	ftpClose(&ftpContext);
	// send report
	reportMessage = mqtt_json_make_fw_update_result(deviceName, serverInfo->serverIp, serverInfo->fileName, (int32_t)status);
	mqttPublishMsg(MQTT_EVENT_TOPIC, reportMessage, strlen(reportMessage));
	free(reportMessage);
	free(serverInfo->fileName);
	free(serverInfo->serverIp);
	serverInfo->fileName = NULL;
	serverInfo->serverIp = NULL;
	serverInfo->fileSize = 0;
    if (imageGetStatus == true)
	{
		TRACE_INFO("Image has been successfully received\r\n");
		// send mqtt response message
		osDelayTask(1000);
		// write key to information partition to inform bootloader about new image
		if (IFLASH_CopyRamToFlash(INFORMATION_START_ADDR, imageUpdateKey, sizeof(imageUpdateKey)))
		{
			TRACE_INFO("Write flash key error\r\n");
		}
		else
		{
			// reset system to let boot loader handle the transfer
			hal_system_reset();
		}
	}
	else 
	{
		TRACE_INFO("Image received failed\r\n");
	}
    vTaskDelete(NULL);
}

void FTP_SetInformation(const char* fileName, const char* serverIp, uint32_t fileSize)
{
	char *name;
	char *server;
	name = malloc(strlen(fileName) + 1);
	server = malloc(strlen(serverIp) + 1);
	memcpy(name, fileName, strlen(fileName));
	memcpy(server, serverIp, strlen(serverIp));
	name[strlen(fileName)] = 0;
	server[strlen(serverIp)] = 0;
	firmwareInfo.fileSize = fileSize;
	firmwareInfo.fileName = name;
	firmwareInfo.serverIp = server;
}

void FTP_StartFirmwareUpdate(const char* fileName, const char* serverIp, uint32_t fileSize)
{
	if ((fileName == NULL) || (serverIp == NULL) || (fileSize == 0) || (fileSize > FTP_FIRMWARE_MAX_SIZE))
	{
		TRACE_INFO("Invalid firmware value\r\n");
		return;
	}
	FTP_SetInformation(fileName, serverIp, fileSize);
	if (xTaskCreate(FTP_FirmwareUpdateTask, "ftp firmware task", 1024, (void*)&firmwareInfo, tskIDLE_PRIORITY + 1, &ftpFirmwareTask) != pdPASS)
	{
		TRACE_INFO("Create ftp task failed\r\n");
	}
}