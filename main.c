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
#include "frdm_k66f.h"
#include "os_port.h"
#include "core/net.h"
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
#include "ethernet.h"
#include "test.h"
#include "quectel_m26.h"

#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"

#if (USERDEF_CLIENT_SNMP == ENABLED)
#include "snmp_client.h"
#endif

/**
* @brief Main entry point
* @return Unused value
**/

int_t main(void)
{
    error_t error;
    NetInterface *interface;
    OsTask *task;
    
    BOARD_BootClockHSRUN();
    //Update system core clock
    //   SystemCoreClockUpdate(
    BOARD_InitPins();
    BOARD_InitLEDs();
    BOARD_InitLCD();
    BOARD_InitUARTs();
    BOARD_InitI2C();
    BOARD_InitSPIs();
    BOARD_InitIOs();
    BOARD_InitBUTTONs();
    BOARD_InitGPRS();
#if (USERDEF_DEBUG_USING == ENABLED) 
    BOARD_InitDebugConsole();
#else
#ifdef DEBUG_CONSOLE_UART4
    BOARD_InitDebugConsole();
#endif
#endif
      //Start-up message
    TRACE_INFO("\r\n");
    TRACE_INFO("**********************************\r\n");
    TRACE_INFO("*** Site Monitoring Device ***\r\n");
    TRACE_INFO("**********************************\r\n");
    TRACE_INFO("\r\n");
    // Init Led
    AppLedInit();
    // Init IO Pin
    AppIoInit();
#if (USERDEF_GPRS == ENABLED)
    gprs_init();
#endif
    
#if (USERDEF_USER_INTERFACE == ENABLED)  
    AppInitUserInterface();
#endif  
    //Initialize kernel
    osInitKernel();
#if (USERDEF_ADC_TASK == ENABLED)
    //ADC initialization
    AppInitAdc();
#endif
    // Init Ethernet module
    interface = EthernetInit();

#if (USERDEF_USER_INTERFACE == ENABLED)
    //	Init_RS485_UART();
#endif
#if (USERDEF_CLIENT_SNMP == ENABLED)
    SnmpInitMib();
    error = SnmpInitClient(interface);
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
    // Declare application Task and timer
    UserTaskInit();
    //Start the execution of tasks
    osStartKernel();
    //This function should never return
    return 0;
}

