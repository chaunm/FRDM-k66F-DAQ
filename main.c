/**
* @file main.c
* @brief Main routine
*
* @section License
* ^^(^____^)^^
*
**/

//Dependencies
#include <stdlib.h>
#include "mk66f18.h"
#include "os_port.h"
#include "core/net.h"
#include "core/ping.h"
#include "ipv6/slaac.h"
#include "http/http_server.h"
#include "http/mime.h"
#include "path.h"
#include "date_time.h"
#include "resource_manager.h"
#include "debug.h"
#include "fsl_debug_console.h"
#include "variables.h"
#include "user_task.h"
#include "app_init.h"
#include "app_ethernet.h"
#include "test.h"
#include "hal_system.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"

#if (USERDEF_CLIENT_SNMP == ENABLED)
#include "snmp_client.h"
#endif

#if (USERDEF_MQTT_CLIENT == ENABLED)
#include "mqtt_client/app_mqtt_client.h"
#endif

#include "ftp.h"

// inclusion for ppp
#include "modem_Interface.h"
#include "modem.h"

/**
* @brief Main entry point
* @return Unused value
**/
NetInterface *ethernetInterface;
NetInterface *pppInterface;

int_t main(void)
{
    error_t error;  
    OsTask *task;
    
    BOARD_BootClockRUN();
    BOARD_InitPins();
    BOARD_InitLEDs();
    BOARD_InitLCD();
    BOARD_InitUARTs();
    BOARD_InitI2C();
    BOARD_InitSPIs();
    BOARD_InitIOs();
    BOARD_InitBUTTONs();
    BOARD_InitGPRS();
#ifdef DEBUG_CONSOLE_UART4
    BOARD_InitDebugConsole();
#endif
      //Start-up message
    TRACE_INFO("\r\n");
    TRACE_INFO("***********************************\r\n");
    TRACE_INFO("***** Site Monitoring Device ******\r\n");
    TRACE_INFO("***********************************\r\n");
    TRACE_INFO("Firmware verison %s, build %s %s\r\n\r\n", __FIRMWARE_VERSION, __DATE__, __TIME__);
    TRACE_INFO("\r\n");
    TRACE_INFO("Reset cause SRS0: 0x%02X SRS1: 0x%02X\r\n", RCM->SRS0, RCM->SRS1);
    // Init Led
    AppLedInit();
    // Init IO Pin
    AppIoInit();

#if (USERDEF_USER_INTERFACE == ENABLED)  
    AppInitUserInterface();
#endif  
        
    //Initialize kernel
    osInitKernel();
#if (USERDEF_GPRS == ENABLED)
    gprs_init();
#endif
    
#if (USERDEF_ADC_TASK == ENABLED)
    AppInitAdc();
#endif
    ethernetInterface = EthernetInit();
    pppInterface = ModemInterfaceInit();
/********** Create SNMP Task *************/
#if (USERDEF_CLIENT_SNMP == ENABLED)
    SnmpInitMib();
    error = SnmpInitClient(ethernetInterface);
    error = SnmpInitClient(pppInterface);
    //Create TrapSend task
    if (error == NO_ERROR)
    {
      task = osCreateTask("TrapSend", SnmpSendTrapTask, NULL, 500, OS_TASK_PRIORITY_NORMAL);
      //Failed to create the task?
      if(task == OS_INVALID_HANDLE)
      {
        //Debug message
        TRACE_ERROR("Failed to create task!\r\n");
      }
    }
    else
      TRACE_ERROR("Failed to create SNMP Client!\r\n");
#endif
 /********** Create MQTT Task ***********/
#if (USERDEF_MQTT_CLIENT == ENABLED)
    if (error == NO_ERROR)
    {
        task = osCreateTask("mqtt_task", mqttClientTask, NULL, MQTT_CLIENT_TASK_STACK_SIZE, OS_TASK_PRIORITY_NORMAL);
      //Failed to create the task?
      if(task == OS_INVALID_HANDLE)
      {
        //Debug message
        TRACE_ERROR("Failed to create task!\r\n");
      }
    }
    else
      TRACE_ERROR("Failed to create SNMP Client!\r\n");
#endif
    UserTaskInit();    
    //Start the execution of tasks
    osStartKernel();
    //This function should never return
    return 0;
}

