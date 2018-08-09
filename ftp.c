#include "net_config.h"
#include "core/net.h"
#include "ftp/ftp_client.h"
#include "ftp.h"
#include "debug.h"
// FreeRTOS inclustions
#include "os_port_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "internal_flash.h"
#include "hal_system.h"
#include <stdlib.h>

#define FTP_USER		"DAQ_FIRMWARE"
#define FTP_PSW			"1234@5678"
#define FIRMWARE_FILE	"DAQ_IMAGE.bin"
#define FIRMWARE_KEY_1	0x55AAAA55
#define FIRMWARE_KEY_2 	0x2112FEEF

uint8_t firmwareBuffer[FTP_FIRMWARE_BUFFER_SIZE];
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
	bool imageGetStatus;
    size_t length;   
	size_t totalRead;
	imageGetStatus = false;
	uint32_t imageUpdateKey[2] = {FIRMWARE_KEY_1, FIRMWARE_KEY_2};
	uint32_t flashAddr = IMAGE_START_ADDR;
    //Debug message
    TRACE_INFO("\r\n\r\nResolving server name...\r\n");
    error = getHostByName(NULL, serverInfo->serverIp, &ipAddr, 0);
    if(error)
    {
        //Debug message
        TRACE_INFO("Failed to resolve server name!\r\n");
		free(serverInfo->fileName);
		free(serverInfo->serverIp);
        vTaskDelete(NULL);
    }
    
    //Debug message
    TRACE_INFO("Connecting to FTP server %s\r\n", ipAddrToString(&ipAddr, NULL));
    //Connect to the FTP server
    error = ftpConnect(&ftpContext, NULL, &ipAddr, 21, FTP_NO_SECURITY | FTP_PASSIVE_MODE);
    
    if(error)
    {
        //Debug message
        TRACE_INFO("Failed to connect to FTP server!\r\n");
		free(serverInfo->fileName);
		free(serverInfo->serverIp);
        //Exit immediately
        vTaskDelete(NULL);
    }
    
    //Debug message
    TRACE_INFO("connect to ftp server succeed\r\n");
	// Initialize flash before updating firmare
	IFLASH_Init();
	IFLASH_Erase(IMAGE_START_ADDR, IMAGE_SIZE);
	totalRead = 0;
    do
    {
        //Login to the FTP server using the provided username and password
        error = ftpLogin(&ftpContext, FTP_USER, FTP_PSW, "");
        if(error) break;
        //Open file
        error = ftpOpenFile(&ftpContext, FIRMWARE_FILE, FTP_FOR_READING | FTP_BINARY_TYPE);
        if(error) break;
        
        while(1)
        {
            //Read data
            error = ftpReadFile(&ftpContext, firmwareBuffer, FTP_FIRMWARE_BUFFER_SIZE, &length, 0);
			totalRead += length;
            //End of file?
            if(error) 
				break;
			// data read size must be evenly devided by 4 since 4 is the smallest size can be write to flash
			if (((length % 4) != 0) && (totalRead != serverInfo->fileSize))
			{
				break;
			}
			// write image to flash
			IFLASH_CopyRamToFlash(flashAddr, (uint32_t*)firmwareBuffer, length);
        }
        if (totalRead != serverInfo->fileSize)
		{
			TRACE_INFO("Read file error \r\n");
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
	ftpClose(&ftpContext);
	free(serverInfo->fileName);
	free(serverInfo->serverIp);
    //Debug message
    TRACE_INFO("Connection closed...\r\n");
    if (imageGetStatus == true)
	{
		TRACE_INFO("Image has been successfully received\r\n");
		// send mqtt response message
		osDelayTask(1000);
		// write key to information partition to inform bootloader about new image
		IFLASH_CopyRamToFlash(INFORMATION_START_ADDR, imageUpdateKey, sizeof(imageUpdateKey));
		// reset system to let boot loader handle the transfer
		hal_system_reset();
	}
	else 
	{
		TRACE_INFO("Image received failed");
		//send mqtt response
	}
    //Return status code
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