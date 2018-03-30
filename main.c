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
#include "drivers/mk6x_eth.h"
#include "drivers/ksz8081.h"
#include "dhcp/dhcp_client.h"
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

#if (USERDEF_CLIENT_FTP == ENABLED)
#include "ftp/ftp_client.h"
#endif

#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"

#if (USERDEF_CLIENT_SNMP == ENABLED)
#include "snmp/snmp_agent.h"
#include "mibs/mib2_module.h"
#include "mibs/mib2_impl.h"
#include "oid.h"
#include "private_mib_module.h"
#include "private_mib_impl.h"
#endif

#if (USERDEF_ADC_TASK == ENABLED)
#include "fsl_adc16.h"
#endif

#if (USERDEF_USER_INTERFACE == ENABLED)
#include "lm2068.h"
#include "menu.h"
#include "eeprom_rtc.h"
#include "rs485.h"
#include "access_control.h"
#endif
#include "timers.h"
#if (USERDEF_SW_TIMER == ENABLED)
#include "timers.h"
#endif

#if (USERDEF_GPRS == ENABLED)
#include "quectel_m26.h"
#endif

#if (USERDEF_SNMPCONNECT_MANAGER == ENABLED)
#include "snmpConnect_manager.h"
#endif

/*******************************************************************************
* Definitions
******************************************************************************/
//Application configuration
#define APP_MAC_ADDR "00-AB-CD-EF-00-65"

#define APP_USE_DHCP DISABLED
#define APP_IPV4_HOST_ADDR "192.168.100.103"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.100.1"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"

#define APP_USE_SLAAC ENABLED
#define APP_IPV6_LINK_LOCAL_ADDR "fe80::65"
#define APP_IPV6_PREFIX "2001:db8::"
#define APP_IPV6_PREFIX_LENGTH 64
#define APP_IPV6_GLOBAL_ADDR "2001:db8::65"
#define APP_IPV6_ROUTER "fe80::1"
#define APP_IPV6_PRIMARY_DNS "2001:4860:4860::8888"
#define APP_IPV6_SECONDARY_DNS "2001:4860:4860::8844"

#define APP_HTTP_MAX_CONNECTIONS 4

#if (USERDEF_CLIENT_SNMP == ENABLED)
#define APP_SNMP_ENTERPRISE_OID "1.3.6.1.4.1.45796.1.16"//"1.3.6.1.4.1.8072.9999.9998"//
#define APP_SNMP_CONTEXT_ENGINE "\x80\x00\x00\x00\x01\x02\x03\x04"
#define APP_SNMP_TRAP_DEST_IP_ADDR "192.168.100.25"//"117.6.55.97"//
#endif

#if (USERDEF_ADC_TASK == ENABLED)
#define BOARD_ADC16_0_BASE ADC0
#define BOARD_ADC16_0_CHANNEL_GROUP 0U
#define BOARD_ADC16_1_BASE ADC1
#define BOARD_ADC16_1_CHANNEL_GROUP 0U

#define BOARD_ADC16_0_USER_CHANNEL1 22U
#define BOARD_ADC16_0_USER_CHANNEL2 21U
#define BOARD_ADC16_0_USER_CHANNEL3 23U
#define BOARD_ADC16_1_USER_CHANNEL1 18U
#define BOARD_ADC16_1_USER_CHANNEL2 23U
#endif

#if (USERDEF_SW_TIMER == ENABLED)
/* The software timer period. */
#define SW_TIMER_PERIOD_MS (1000 / portTICK_PERIOD_MS)
#endif

//Global variables


#if (USERDEF_ADC_TASK == ENABLED)
adc16_config_t adc16ConfigStruct;
adc16_channel_config_t adc16ChannelConfigStruct;
uint32_t adcValue[10];
#endif

#if (USERDEF_SW_TIMER == ENABLED)
TimerHandle_t SwTimerHandle = NULL;
#endif

#if (APP_USE_DHCP == ENABLED)
DhcpClientSettings dhcpClientSettings;
DhcpClientCtx dhcpClientContext;
#endif

#if (USERDEF_CLIENT_SNMP == ENABLED)
SnmpAgentSettings snmpAgentSettings;
SnmpAgentContext snmpAgentContext;
#endif

/*******************************************************************************
* Prototypes
******************************************************************************/
#if (USERDEF_SW_TIMER == ENABLED)
/* The callback function. */
static void SwTimerCallback(TimerHandle_t xTimer);
#endif

#if (USERDEF_CLIENT_SNMP == ENABLED)
error_t snmpAgentRandCallback(uint8_t *data, size_t length);
void trapsendTest(void);
void Trap_Send_Type_1(SnmpAgentContext *context, const IpAddr *destIpAddr,
                      SnmpVersion version, const char_t *username, uint_t genericTrapType,
                      uint_t specificTrapCode, const SnmpTrapObject *objectList, uint_t objectListSize ,
                      const char_t *str, uint8_t *oid, size_t maxOidLen, size_t *oidLen,
                      uint32_t* pui32value, uint16_t number, uint8_t alarmVal1, uint8_t alarmVal2, uint8_t normalVal);
void Trap_Send_Type_2(SnmpAgentContext *context, const IpAddr *destIpAddr,
                      SnmpVersion version, const char_t *username, uint_t genericTrapType,
                      uint_t specificTrapCode, const SnmpTrapObject *objectList, uint_t objectListSize ,
                      uint32_t* pui32value_new, uint32_t* pui32value_old);
#endif

#if (USERDEF_CLIENT_FTP == ENABLED)
error_t ftpClientTest(void);
#endif

#if (USERDEF_CLIENT_SNMP == ENABLED)
void trapSendTask(void *param)
{
    
    //	//Endless loop
    while(1)
    {
        
        trapsendTest();
//        snmpConnectManagerSendPing();
//        //SW2 button pressed?
//        if(!BUTTON_ENTER_READ() && (sMenu_Control.menu == _DEVICE_INFO))
//        {
//          //trapsendTest();
//          //Wait for the SW2 button to be released
//          while(!BUTTON_ENTER_READ());
//        }
//        //Loop delay
//        osDelayTask(100);
        osDelayTask(3000);
    }
}
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
    MacAddr macAddr;
#if (APP_USE_DHCP == DISABLED)
    Ipv4Addr ipv4Addr;
#endif
    
#if (USERDEF_CLIENT_SNMP == ENABLED)
    size_t oidLen;
    uint8_t oid[SNMP_MAX_OID_SIZE];
#endif
    /* Define the init structure */
    gpio_pin_config_t out_config = {
        kGPIO_DigitalOutput, 0,
    };
    gpio_pin_config_t in_config = {
        kGPIO_DigitalInput, 0,
    };
    
    BOARD_BootClockHSRUN();
    //Update system core clock
    //   SystemCoreClockUpdate();
    
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
    LED_STATUS_INIT(1);
    LED_RUN_INIT(1);
    LED_CON_GPRS_INIT(1);
    LED_CON_ETH_INIT(1);
    LED_ALARM_INIT(1);
    
    IO_MUX_S1_INIT(0);
    IO_MUX_S2_INIT(0);
    IO_MUX_S3_INIT(0);
    IO_MUX_S0_INIT(0);
    
    IO_WAITMCU_1_INIT(0);
    IO_WAITMCU_2_INIT(0);
    IO_WAITMCU_3_INIT(0);
    IO_WAITMCU_4_INIT(0);
    IO_WAITMCU_5_INIT(0);
    IO_OPENDOOR_MCU_INIT(0);
    IO_ALARM_LIGHT_INIT(0);
    IO_ALARM_SPEAKER_INIT(0);
    
#if (USERDEF_GPRS == ENABLED)
    gprs_init();
#endif
    
#if (USERDEF_USER_INTERFACE == ENABLED)
    
    /* Init output */
    //GLCD
    GPIO_PinInit(GPIOC, 4u, &out_config);
    GPIO_PinInit(LCD_BL_PORT, LCD_BL_PIN, &out_config);
    GPIO_PinInit(LCD_RES_PORT, LCD_RES_PIN, &out_config);
    GPIO_PinInit(LCD_A0_PORT, LCD_A0_PIN, &out_config);
    GPIO_PinInit(LCD_CS_PORT, LCD_CS_PIN, &out_config);
    GPIO_PinInit(LCD_RD_PORT, LCD_RD_PIN, &out_config);
    GPIO_PinInit(LCD_WR_PORT, LCD_WR_PIN, &out_config);
    GPIO_PinInit(LCD_DB0_PORT, LCD_DB0_PIN, &out_config);
    GPIO_PinInit(LCD_DB1_PORT, LCD_DB1_PIN, &out_config);
    GPIO_PinInit(LCD_DB2_PORT, LCD_DB2_PIN, &out_config);
    GPIO_PinInit(LCD_DB3_PORT, LCD_DB3_PIN, &out_config);
    GPIO_PinInit(LCD_DB4_PORT, LCD_DB4_PIN, &out_config);
    GPIO_PinInit(LCD_DB5_PORT, LCD_DB5_PIN, &out_config);
    GPIO_PinInit(LCD_DB6_PORT, LCD_DB6_PIN, &out_config);
    GPIO_PinInit(LCD_DB7_PORT, LCD_DB7_PIN, &out_config);
    //EEPROM-RTC
    GPIO_PinInit(SDA_PORT, SDA_PIN, &out_config);
    GPIO_PinInit(SCL_PORT, SCL_PIN, &out_config);
    /* Init input */
    //KEY BOARD
    GPIO_PinInit(KEY_1_PORT, KEY_1_PIN, &in_config);
    GPIO_PinInit(KEY_2_PORT, KEY_2_PIN, &in_config);
    GPIO_PinInit(KEY_3_PORT, KEY_3_PIN, &in_config);
    GPIO_PinInit(KEY_4_PORT, KEY_4_PIN, &in_config);
    Init_I2CE();
    Init_All_Variable();
    Init_RS485_UART();
#endif
    
    //Initialize kernel
    osInitKernel();
    
    //Start-up message
    TRACE_INFO("\r\n");
    TRACE_INFO("**********************************\r\n");
    TRACE_INFO("*** Site Monitoring Device ***\r\n");
    TRACE_INFO("**********************************\r\n");
    TRACE_INFO("\r\n");
    //ADC initialization
#if (USERDEF_ADC_TASK == ENABLED)
    /*
    * adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref;
    * adc16ConfigStruct.clockSource = kADC16_ClockSourceAsynchronousClock;
    * adc16ConfigStruct.enableAsynchronousClock = true;
    * adc16ConfigStruct.clockDivider = kADC16_ClockDivider8;
    * adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;
    * adc16ConfigStruct.longSampleMode = kADC16_LongSampleDisabled;
    * adc16ConfigStruct.enableHighSpeed = false;
    * adc16ConfigStruct.enableLowPower = false;
    * adc16ConfigStruct.enableContinuousConversion = false;
    */
    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    
    adc16ConfigStruct.enableHighSpeed = true;
    adc16ConfigStruct.enableContinuousConversion = true;
    //========================= ADC0 Initialization ================================//
    ADC16_Init(BOARD_ADC16_0_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(BOARD_ADC16_0_BASE, false); /* Make sure the software trigger is used. */
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    if (kStatus_Success == ADC16_DoAutoCalibration(BOARD_ADC16_0_BASE))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
    //========================= ADC1 Initialization ================================//
    
    ADC16_Init(BOARD_ADC16_1_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(BOARD_ADC16_1_BASE, false); /* Make sure the software trigger is used. */
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    if (kStatus_Success == ADC16_DoAutoCalibration(BOARD_ADC16_1_BASE))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
    
    adc16ChannelConfigStruct.channelNumber = BOARD_ADC16_1_USER_CHANNEL1;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
#endif
    
#if (USERDEF_USER_INTERFACE == ENABLED)
    //	Init_RS485_UART();
#endif
    
#if (USERDEF_CLIENT_SNMP == ENABLED)
    //Standard MIB-II initialization
    error = mib2Init();
    //Any error to report?
    if(error)
    {
        //Debug message
        TRACE_ERROR("Failed to initialize MIB!\r\n");
    }
    //Private MIB initialization
    error = privateMibInit();
    //Any error to report?
    if(error)
    {
        //Debug message
        TRACE_ERROR("Failed to initialize MIB!\r\n");
    }
#endif
    //TCP/IP stack initialization
    error = netInit();
    //Any error to report?
    if(error)
    {
        //Debug message
        TRACE_ERROR("Failed to initialize TCP/IP stack!\r\n");
    }
    
    //Configure the first Ethernet interface
    interface = &netInterface[0];
    
    //Set interface name
    netSetInterfaceName(interface, "eth0");
    //Set host name
    netSetHostname(interface, "SiteMonitor");
    //Select the relevant network adapter
    netSetDriver(interface, &mk6xEthDriver);
    netSetPhyDriver(interface, &ksz8081PhyDriver);
    //Set host MAC address
    macStringToAddr(APP_MAC_ADDR, &macAddr);
    netSetMacAddr(interface, &macAddr);
    
    //Initialize network interface
    error = netConfigInterface(interface);
    //Any error to report?
    if(error)
    {
        //Debug message
        TRACE_ERROR("Failed to configure interface %s!\r\n", interface->name);
    }
    
#if (IPV4_SUPPORT == ENABLED)
#if (APP_USE_DHCP == ENABLED)
    //Get default settings
    dhcpClientGetDefaultSettings(&dhcpClientSettings);
    //Set the network interface to be configured by DHCP
    dhcpClientSettings.interface = interface;
    //Disable rapid commit option
    dhcpClientSettings.rapidCommit = FALSE;
    
    //DHCP client initialization
    error = dhcpClientInit(&dhcpClientContext, &dhcpClientSettings);
    //Failed to initialize DHCP client?
    if(error)
    {
	//Debug message
	TRACE_ERROR("Failed to initialize DHCP client!\r\n");
    }
    
    //Start DHCP client
    error = dhcpClientStart(&dhcpClientContext);
    //Failed to start DHCP client?
    if(error)
    {
	//Debug message
	TRACE_ERROR("Failed to start DHCP client!\r\n");
    }
#else
    //Set IPv4 host address
    ipv4StringToAddr((const char*)sMenu_Variable.ucIP, &ipv4Addr);//"192.168.100.104"
    ipv4SetHostAddr(interface, ipv4Addr);
    
    //Set subnet mask
    ipv4StringToAddr((const char*)sMenu_Variable.ucSN, &ipv4Addr);//"255.255.255.0"
    ipv4SetSubnetMask(interface, ipv4Addr);
    
    //Set default gateway
    ipv4StringToAddr((const char*)sMenu_Variable.ucGW, &ipv4Addr);//"192.168.100.1"
    ipv4SetDefaultGateway(interface, ipv4Addr);
    
    //Set primary and secondary DNS servers
    ipv4StringToAddr(APP_IPV4_PRIMARY_DNS, &ipv4Addr);
    ipv4SetDnsServer(interface, 0, ipv4Addr);
    ipv4StringToAddr(APP_IPV4_SECONDARY_DNS, &ipv4Addr);
    ipv4SetDnsServer(interface, 1, ipv4Addr);
#endif
#endif
    //==================================================SNMP CLIENT CONFIG===========================================//
#if (USERDEF_CLIENT_SNMP == ENABLED)
    snmpAgentGetDefaultSettings(&snmpAgentSettings);
    snmpAgentSettings.interface = interface;
    snmpAgentSettings.versionMin = SNMP_VERSION_1;
    snmpAgentSettings.versionMax = SNMP_VERSION_2C;
    
#if (SNMP_V3_SUPPORT == ENABLED)
    snmpAgentSettings.versionMax = SNMP_VERSION_3;
    snmpAgentSettings.randCallback = snmpAgentRandCallback;
#endif
    
    //SNMP agent initialization
    error = snmpAgentInit(&snmpAgentContext, &snmpAgentSettings);
    if(error)
    {
        //Debug message
        TRACE_ERROR("Failed to initialize SNMP agent!\r\n");
    }
    
    //Load standard MIB-II
    snmpAgentLoadMib(&snmpAgentContext, &mib2Module);
    //Load private MIB
    snmpAgentLoadMib(&snmpAgentContext, &privateMibModule);
    oidFromString(APP_SNMP_ENTERPRISE_OID, oid, sizeof(oid), &oidLen);
    //Set enterprise OID
    snmpAgentSetEnterpriseOid(&snmpAgentContext, oid, oidLen);
    
    //Set read-only community string
    snmpAgentCreateCommunity(&snmpAgentContext, "public",
                             SNMP_ACCESS_READ_ONLY);
    
    //Set read-write community string
    snmpAgentCreateCommunity(&snmpAgentContext, "private",
                             SNMP_ACCESS_READ_WRITE);
    
#if (SNMP_V3_SUPPORT == ENABLED)
    //Set context engine identifier
    snmpAgentSetContextEngine(&snmpAgentContext,
                              APP_SNMP_CONTEXT_ENGINE, sizeof(APP_SNMP_CONTEXT_ENGINE) - 1);
    
    //Add a new user
    snmpAgentCreateUser(&snmpAgentContext, "usr-md5-none",
                        SNMP_ACCESS_READ_WRITE, SNMP_KEY_FORMAT_TEXT,
                        SNMP_AUTH_PROTOCOL_MD5, "authkey1",
                        SNMP_PRIV_PROTOCOL_NONE, "");
    
    //Add a new user
    snmpAgentCreateUser(&snmpAgentContext, "usr-md5-aes",
                        SNMP_ACCESS_READ_WRITE, SNMP_KEY_FORMAT_TEXT,
                        SNMP_AUTH_PROTOCOL_MD5, "authkey2",
                        SNMP_PRIV_PROTOCOL_AES, "privkey2");
#endif
    
    //Start SNMP agent
    error = snmpAgentStart(&snmpAgentContext);
    //Failed to start SNMP agent?
    if(error)
    {
        //Debug message
        TRACE_ERROR("Failed to start SNMP agent!\r\n");
    }
    //        //Create user task
    //         task = osCreateTask("SNMP Task", (OsTaskCode) snmpAgentTask, &snmpAgentContext, 1024, OS_TASK_PRIORITY_NORMAL);
    //         //Failed to create the task?
    //         if(task == OS_INVALID_HANDLE)
    //         {
    //            //Debug message
    //            TRACE_ERROR("Failed to create SNMP task!\r\n");
    //         }
    //Create TrapSend task
    task = osCreateTask("TrapSend", trapSendTask, NULL, 500, OS_TASK_PRIORITY_NORMAL);
    //Failed to create the task?
    if(task == OS_INVALID_HANDLE)
    {
        //Debug message
        TRACE_ERROR("Failed to create  task!\r\n");
    }
    
#endif
    //==================================================SNMP CLIENT CONFIG===========================================//

    // Declare application Task and timer
    UserTaskInit();
    //Start the execution of tasks
    osStartKernel();
    
    //This function should never return
    return 0;
}

#if (USERDEF_CLIENT_SNMP == ENABLED)
/**
* @brief Random data generation callback function
* @param[out] data Buffer where to store the random data
* @param[in] lenght Number of bytes that are required
* @return Error code
**/

error_t snmpAgentRandCallback(uint8_t *data, size_t length)
{
    size_t i;
    
    //Generate some random data
    for(i = 0; i < length; i++)
        data[i] = rand();
    
    //No error to report
    return NO_ERROR;
}

void trapsendTest(void)
{
    error_t error;
    IpAddr destIpAddr;
    SnmpTrapObject trapObjects[65];
    
    //Destination IP address
    ipStringToAddr((const char_t*)sMenu_Variable.ucSIP, &destIpAddr);  
    
    //Add the alarmFireAlarms.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.15.1.0", trapObjects[0].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //Add the siteInfoBTSCode.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    
    Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                     "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,1, trapObjects, 1,                             
                     &privateMibBase.alarmGroup.alarmFireAlarms, 
                     &privateMibBase.alarmGroup.alarmFireAlarms_old);
    //Add the alarmSmokeAlarms.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.15.2.0", trapObjects[1].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //Add the siteInfoBTSCode.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[0].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    
    Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                     "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,2, trapObjects, 1,                              
                     &privateMibBase.alarmGroup.alarmSmokeAlarms, 
                     &privateMibBase.alarmGroup.alarmSmokeAlarms_old);
    //Add the alarmMotionDetectAlarms.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.15.3.0", trapObjects[0].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen); 
    //Add the siteInfoBTSCode.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);       
    
    Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                     "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,3, trapObjects, 1,                             
                     &privateMibBase.alarmGroup.alarmMotionDetectAlarms, 
                     &privateMibBase.alarmGroup.alarmMotionDetectAlarms_old);
    //Add the alarmFloodDetectAlarms.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.15.4.0", trapObjects[0].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //Add the siteInfoBTSCode.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    
    Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                     "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,4, trapObjects, 1,                             
                     &privateMibBase.alarmGroup.alarmFloodDetectAlarms, 
                     &privateMibBase.alarmGroup.alarmFloodDetectAlarms_old);
    //Add the alarmDoorOpenAlarms.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.15.5.0", trapObjects[0].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //Add the siteInfoBTSCode.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    
    Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                     "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,5, trapObjects, 1,
                     &privateMibBase.alarmGroup.alarmDoorOpenAlarms, 
                     &privateMibBase.alarmGroup.alarmDoorOpenAlarms_old);
    //Add the alarmGenFailureAlarms.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.15.6.0", trapObjects[0].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //Add the siteInfoBTSCode.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    
    Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                     "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,6, trapObjects, 1,
                     &privateMibBase.alarmGroup.alarmGenFailureAlarms, 
                     &privateMibBase.alarmGroup.alarmGenFailureAlarms_old);
    //Add the alarmDcThresAlarms.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.15.7.0", trapObjects[0].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //Add the siteInfoBTSCode.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    
    Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                     "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,7, trapObjects, 1,
                     &privateMibBase.alarmGroup.alarmDcThresAlarms, 
                     &privateMibBase.alarmGroup.alarmDcThresAlarms_old);
    //Add the alarmMachineStopAlarms.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.15.8.0", trapObjects[0].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //Add the siteInfoBTSCode.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    
    Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                     "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,8, trapObjects, 1,
                     &privateMibBase.alarmGroup.alarmMachineStopAlarms, 
                     &privateMibBase.alarmGroup.alarmMachineStopAlarms_old);
    //Add the alarmAccessAlarms.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.15.9.0", trapObjects[0].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //Add the siteInfoBTSCode.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    
    Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                     "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,9, trapObjects, 1,
                     &privateMibBase.alarmGroup.alarmAccessAlarms, 
                     &privateMibBase.alarmGroup.alarmAccessAlarms_old);
    //Add the alarmAcThresAlarms.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.15.10.0", trapObjects[0].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //Add the siteInfoBTSCode.0 object to the variable binding list of the message
    oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                  SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    
    Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                     "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,10, trapObjects, 1,
                     &privateMibBase.alarmGroup.alarmAcThresAlarms, 
                     &privateMibBase.alarmGroup.alarmAcThresAlarms_old);
    //======================================= Send AlarmAcessAlarms variable with SiteInfoAccessId ========================//
    
    //        //        Add the battery1Voltage.0 object to the variable binding list of the message
    //          oidFromString("1.3.6.1.4.1.45796.1.15.9.0", trapObjects[0].oid,
    //          SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //          //Add the battery1Voltage.0 object to the variable binding list of the message
    //          oidFromString("1.3.6.1.4.1.45796.1.1.8.0", trapObjects[1].oid,
    //          SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    
    //======================================= Send AlarmAcessAlarms variable with SiteInfoAccessId ========================//
    //        if ((privateMibBase.alarmGroup.alarmFireAlarms != 0)    ||
    //            (privateMibBase.alarmGroup.alarmSmokeAlarms != 0)    ||
    //            (privateMibBase.alarmGroup.alarmMotionDetectAlarms != 0)    ||
    //            (privateMibBase.alarmGroup.alarmFloodDetectAlarms != 0)    ||
    //            (privateMibBase.alarmGroup.alarmDoorOpenAlarms != 0)    ||
    //            (privateMibBase.alarmGroup.alarmGenFailureAlarms != 0)    ||
    //            (privateMibBase.alarmGroup.alarmDcThresAlarms != 0)    ||
    //            (privateMibBase.alarmGroup.alarmMachineStopAlarms != 0)    )
    //        {
    //           //Add the battery1Voltage.0 object to the variable binding list of the message
    //          oidFromString("1.3.6.1.4.1.45796.1.15.1.0", trapObjects[0].oid,
    //          SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
    //          //Add the battery1Voltage.0 object to the variable binding list of the message
    //          oidFromString("1.3.6.1.4.1.45796.1.15.2.0", trapObjects[1].oid,
    //          SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
    //          //Add the battery1Voltage.0 object to the variable binding list of the message
    //          oidFromString("1.3.6.1.4.1.45796.1.15.3.0", trapObjects[2].oid,
    //          SNMP_MAX_OID_SIZE, &trapObjects[2].oidLen);
    //          //Add the battery1Voltage.0 object to the variable binding list of the message
    //          oidFromString("1.3.6.1.4.1.45796.1.15.4.0", trapObjects[3].oid,
    //          SNMP_MAX_OID_SIZE, &trapObjects[3].oidLen);
    //          //Add the battery1Voltage.0 object to the variable binding list of the message
    //          oidFromString("1.3.6.1.4.1.45796.1.15.5.0", trapObjects[4].oid,
    //          SNMP_MAX_OID_SIZE, &trapObjects[4].oidLen);
    //          //Add the battery1Voltage.0 object to the variable binding list of the message
    //          oidFromString("1.3.6.1.4.1.45796.1.15.6.0", trapObjects[5].oid,
    //          SNMP_MAX_OID_SIZE, &trapObjects[5].oidLen);
    //          //Add the battery1Voltage.0 object to the variable binding list of the message
    //          oidFromString("1.3.6.1.4.1.45796.1.15.7.0", trapObjects[6].oid,
    //          SNMP_MAX_OID_SIZE, &trapObjects[6].oidLen);
    //          //Add the battery1Voltage.0 object to the variable binding list of the message
    //          oidFromString("1.3.6.1.4.1.45796.1.15.8.0", trapObjects[7].oid,
    //          SNMP_MAX_OID_SIZE, &trapObjects[7].oidLen);
    //          
    //                  //Send a SNMP trap
    //          error = snmpAgentSendTrap(&snmpAgentContext, &destIpAddr, SNMP_VERSION_2C,
    //          "public",SNMP_TRAP_ENTERPRISE_SPECIFIC , 7, trapObjects, 8); //
    //        }
    if (trapStatus_TimePeriod >= 30)
    {
        //============================= Site Info ============================================//
	//Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
	//Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.2.0", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
	//Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.3.0", trapObjects[2].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[2].oidLen);
	//Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.4.0", trapObjects[3].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[3].oidLen);
	//Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.5.0", trapObjects[4].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[4].oidLen);
	//Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.6.0", trapObjects[5].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[5].oidLen);
	//Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.7.0", trapObjects[6].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[6].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.8.0", trapObjects[7].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[7].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.9.0", trapObjects[8].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[8].oidLen);        
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[9].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[9].oidLen);
        
        //Send a SNMP trap
	error = snmpAgentSendTrap(&snmpAgentContext, &destIpAddr, SNMP_VERSION_2C,
                                  "public",SNMP_TRAP_ENTERPRISE_SPECIFIC , 11, trapObjects, 10); //
        
	//Failed to send trap message?
	if(error)
	{
            //Debug message
            TRACE_ERROR("Failed to send SNMP trap message 1!\r\n");
	} else
	{
            //Debug message
            TRACE_INFO("Trap result: %d\r\n", error);
	}
        
        //============================= AC Info ============================================//
	//Add the acPhaseNumber.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.2.1.0", trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
	//Add the acPhaseIndex.1 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.2.2.1.1.1", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
	//Add the acPhaseVolt.1 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.2.2.1.2.1", trapObjects[2].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[2].oidLen);
	//Add the acPhaseAlarmStatus.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.2.2.1.3.1", trapObjects[3].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[3].oidLen);
	//Add the acPhaseCurrent.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.2.2.1.4.1", trapObjects[4].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[4].oidLen);
	//Add the acPhasePower.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.2.2.1.5.1", trapObjects[5].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[5].oidLen);
	//Add the acPhaseFrequency.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.2.2.1.6.1", trapObjects[6].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[6].oidLen);
	//Add the acPhaseThresVolt.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.2.2.1.7.1", trapObjects[7].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[7].oidLen);       
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[8].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[8].oidLen);
        
        //Send a SNMP trap
	error = snmpAgentSendTrap(&snmpAgentContext, &destIpAddr, SNMP_VERSION_2C,
                                  "public",SNMP_TRAP_ENTERPRISE_SPECIFIC , 12, trapObjects, 9); //
        
	//Failed to send trap message?
	if(error)
	{
            //Debug message
            TRACE_ERROR("Failed to send SNMP trap message 2!\r\n");
	} else
	{
            //Debug message
            TRACE_INFO("Trap result: %d\r\n", error);
	}
        
        //============================= Battery Info ============================================//
	//Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.3.1.0", trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
	//Add the battery2Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.3.2.0", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
	//Add the battery1AlarmStatus.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.3.3.0", trapObjects[2].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[2].oidLen);
	//Add the battery2AlarmStatus.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.3.4.0", trapObjects[3].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[3].oidLen);
	//Add the battery1ThresVolt.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.3.5.0", trapObjects[4].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[4].oidLen);
	//Add the battery2ThresVolt.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.3.6.0", trapObjects[5].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[5].oidLen);       
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[6].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[6].oidLen);
        
        //Send a SNMP trap
	error = snmpAgentSendTrap(&snmpAgentContext, &destIpAddr, SNMP_VERSION_2C,
                                  "public",SNMP_TRAP_ENTERPRISE_SPECIFIC , 13, trapObjects, 7); //
        
	//Failed to send trap message?
	if(error)
	{
            //Debug message
            TRACE_ERROR("Failed to send SNMP trap message 1!\r\n");
	} else
	{
            //Debug message
            TRACE_INFO("Trap result: %d\r\n", error);
	}
	//============================= Accessories Info ============================================//
	//Add the airCon1Status.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.1.0", trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
	//Add the airCon1Status.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.2.0", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
	//Add the fan1Status.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.3.0", trapObjects[2].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[2].oidLen);
	//Add the fan2Status.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.4.0", trapObjects[3].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[3].oidLen);
	//Add the doorStatus.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.5.0", trapObjects[4].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[4].oidLen);
        //Add the airConSetTemp1.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.6.0", trapObjects[5].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[5].oidLen);
        //Add the airConSetTemp2.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.7.0", trapObjects[6].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[6].oidLen);
        //Add the airConSetTemp3.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.8.0", trapObjects[7].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[7].oidLen);
        //Add the airConSetTemp4.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.9.0", trapObjects[8].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[8].oidLen);   
        //Add the ledControlStatus.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.5.0", trapObjects[9].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[9].oidLen);
        //Add the speakerControlStatus.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.4.5.0", trapObjects[10].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[10].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[11].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[11].oidLen);
        
        
        //Send a SNMP trap
	error = snmpAgentSendTrap(&snmpAgentContext, &destIpAddr, SNMP_VERSION_2C,
                                  "public",SNMP_TRAP_ENTERPRISE_SPECIFIC , 14, trapObjects, 12); //
        
	//Failed to send trap message?
	if(error)
	{
            //Debug message
            TRACE_ERROR("Failed to send SNMP trap message 1!\r\n");
	} else
	{
            //Debug message
            TRACE_INFO("Trap result: %d\r\n", error);
	}
	//============================= Configuration Info ============================================//
	//Add the configDevIPAddr.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.1.0", trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
	//Add the configDevGWAddr.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.2.0", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
	//Add the configDevSNAddr.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.3.0", trapObjects[2].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[2].oidLen);
	//Add the configDevPort.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.4.0", trapObjects[3].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[3].oidLen);
	//Add the configServerIPAddr.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.5.0", trapObjects[4].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[4].oidLen);
	//Add the configServerPort.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.6.0", trapObjects[5].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[5].oidLen);
	//Add the configRefreshInterval.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.7.0", trapObjects[6].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[6].oidLen);
	//Add the configUserNumber.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.8.0", trapObjects[7].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[7].oidLen);
	//Add the configAccNumber.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.9.0", trapObjects[8].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[8].oidLen);
        
	//Add the configAccIndex.1 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.1.1", trapObjects[9].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[9].oidLen);
	//Add the configAccName.1 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.2.1", trapObjects[10].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[10].oidLen);
	//Add the configAccAuthkey.1 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.3.1", trapObjects[11].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[11].oidLen);
	//Add the configAccPrivkey.1 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.4.1", trapObjects[12].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[12].oidLen);
	//Add the configAccAuthType.1 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.5.1", trapObjects[13].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[13].oidLen);
	//Add the configAccPrivType.1 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.6.1", trapObjects[14].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[14].oidLen);
        
        
	//Add the configAccIndex.2 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.1.2", trapObjects[15].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[15].oidLen);
	//Add the configAccName.2 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.2.2", trapObjects[16].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[16].oidLen);
	//Add the configAccAuthkey.2 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.3.2", trapObjects[17].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[17].oidLen);
	//Add the configAccPrivkey.2 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.4.2", trapObjects[18].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[18].oidLen);
	//Add the configAccAuthType.2 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.5.2", trapObjects[19].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[19].oidLen);
	//Add the configAccPrivType.2 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.6.2", trapObjects[20].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[20].oidLen);
        
        
	//Add the configAccIndex.3 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.1.3", trapObjects[21].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[21].oidLen);
	//Add the configAccName.3 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.2.3", trapObjects[22].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[22].oidLen);
	//Add the configAccAuthkey.3 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.3.3", trapObjects[23].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[23].oidLen);
	//Add the configAccPrivkey.3 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.4.3", trapObjects[24].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[24].oidLen);
	//Add the configAccAuthType.3 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.5.3", trapObjects[25].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[25].oidLen);
	//Add the configAccPrivType.3 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.10.1.6.3", trapObjects[26].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[26].oidLen);
        
	//Add the configAccessIdNumber.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.11.0", trapObjects[27].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[27].oidLen);
	//Add the configAccessIdIndex.1 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.12.1.1.1", trapObjects[28].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[28].oidLen);
	//Add the configAccessIdCard.1 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.12.1.2.1", trapObjects[29].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[29].oidLen);
	//Add the configAccessIdIndex.2 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.12.1.1.2", trapObjects[30].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[30].oidLen);
	//Add the configAccessIdCard.2 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.12.1.2.2", trapObjects[31].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[31].oidLen);
	//Add the configAccessIdIndex.3 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.12.1.1.3", trapObjects[32].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[32].oidLen);
	//Add the configAccessIdCard.3 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.12.1.2.3", trapObjects[33].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[33].oidLen);
	//Add the configAccessIdIndex.4 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.12.1.1.4", trapObjects[34].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[34].oidLen);
	//Add the configAccessIdCard.4 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.12.1.2.4", trapObjects[35].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[35].oidLen);
	//Add the configAccessIdIndex.5 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.12.1.1.5", trapObjects[36].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[36].oidLen);
	//Add the configAccessIdCard.5 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.14.12.1.2.5", trapObjects[37].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[37].oidLen);       
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[38].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[38].oidLen);
        
        
        
        //Send a SNMP trap
	error = snmpAgentSendTrap(&snmpAgentContext, &destIpAddr, SNMP_VERSION_2C,
                                  "public",SNMP_TRAP_ENTERPRISE_SPECIFIC , 15, trapObjects, 39); //
        
	//Failed to send trap message?
	if(error)
	{
            //Debug message
            TRACE_ERROR("Failed to send SNMP trap message 1!\r\n");
	} else
	{
            //Debug message
            TRACE_INFO("Trap result: %d\r\n", error);
	}
        //============================= Alarm Info ============================================//
	//Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.15.1.0", trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
	//Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.15.2.0", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
	//Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.15.3.0", trapObjects[2].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[2].oidLen);
	//Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.15.4.0", trapObjects[3].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[3].oidLen);
	//Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.15.5.0", trapObjects[4].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[4].oidLen);
	//Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.15.6.0", trapObjects[5].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[5].oidLen);
	//Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.15.7.0", trapObjects[6].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[6].oidLen);
	//Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.15.8.0", trapObjects[7].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[7].oidLen);
        //Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.15.9.0", trapObjects[8].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[8].oidLen);
        //Add the battery1Voltage.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.15.10.0", trapObjects[9].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[9].oidLen);       
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
	oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[10].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[10].oidLen);
        
        
        //Send a SNMP trap
	error = snmpAgentSendTrap(&snmpAgentContext, &destIpAddr, SNMP_VERSION_2C,
                                  "public",SNMP_TRAP_ENTERPRISE_SPECIFIC , 16, trapObjects, 11); //
        
	//Failed to send trap message?
	if(error)
	{
            //Debug message
            TRACE_ERROR("Failed to send SNMP trap message 1!\r\n");
	} else
	{
            //Debug message
            TRACE_INFO("Trap result: %d\r\n", error);
	}
        trapStatus_TimePeriod = 0;
    }
    
    
    //	//Add the ifNum object to the variable binding list of the message
    //	oidFromString("1.3.6.1.2.1.11.15.0", trapObjects[0].oid,
    //	SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);//1.3.6.1.2.1.2.2.1.2.1
    //	//Add the ifPhysAddress.1 object to the variable binding list of the message
    //	oidFromString("1.3.6.1.4.1.8072.9999.9999.1.1.0", trapObjects[1].oid,
    //	SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);//1.3.6.1.2.1.2.2.1.6.1
    //	//Add the ifDecr.1 object to the variable binding list of the message
    //	oidFromString("1.3.6.1.2.1.2.2.1.2.1", trapObjects[2].oid,
    //	SNMP_MAX_OID_SIZE, &trapObjects[2].oidLen);//1.3.6.1.2.1.2.2.1.6.1
    //============================= Trap Info ============================================//
}
#endif

#if (USERDEF_CLIENT_FTP == ENABLED)
/**
* @brief FTP client test routine
* @return Error code
**/

error_t ftpClientTest(void)
{
    error_t error;
    size_t length;
    IpAddr ipAddr;
    FtpClientContext ftpContext;
    static char_t buffer[256];
    
    //Debug message
    TRACE_INFO("\r\n\r\nResolving server name...\r\n");
    error = getHostByName(NULL, "192.168.100.25", &ipAddr, 0);
    if(error)
    {
        //Debug message
        TRACE_INFO("Failed to resolve server name!\r\n");
        return error;
    }
    
    //Debug message
    TRACE_INFO("Connecting to FTP server %s\r\n", ipAddrToString(&ipAddr, NULL));
    //Connect to the FTP server
    error = ftpConnect(&ftpContext, NULL, &ipAddr, 21, FTP_NO_SECURITY | FTP_PASSIVE_MODE);
    
    if(error)
    {
        //Debug message
        TRACE_INFO("Failed to connect to FTP server!\r\n");
        //Exit immediately
        return error;
    }
    
    //Debug message
    TRACE_INFO("Successful connection\r\n");
    do
    {
        //Login to the FTP server using the provided username and password
        error = ftpLogin(&ftpContext, "sonlq4", "123", "");
        if(error) break;
        //Open file
        error = ftpOpenFile(&ftpContext, "FRDM-K66F.hex", FTP_FOR_READING | FTP_BINARY_TYPE);
        if(error) break;
        
        while(1)
        {
            //Read data
            error = ftpReadFile(&ftpContext, buffer, sizeof(buffer) - 1, &length, 0);
            //End of file?
            if(error) break;
            
            //terminate the string with a NULL character
            buffer[length] = '\0';
            TRACE_INFO("%s", buffer);
        }
        
        TRACE_INFO("\r\n");
        //Close the file
        error = ftpCloseFile(&ftpContext);
        
        //End of exception handling block
    } while(0);
    
    //Close the connection
    ftpClose(&ftpContext);
    //Debug message
    TRACE_INFO("Connection closed...\r\n");
    
    //Return status code
    return error;
}
#endif

void Trap_Send_Type_1(SnmpAgentContext *context, const IpAddr *destIpAddr,
                      SnmpVersion version, const char_t *username, uint_t genericTrapType,
                      uint_t specificTrapCode, const SnmpTrapObject *objectList, uint_t objectListSize ,
                      const char_t *str, uint8_t *oid, size_t maxOidLen, size_t *oidLen,
                      uint32_t* pui32value, uint16_t number, uint8_t alarmVal1, uint8_t alarmVal2, uint8_t normalVal)
{  
    //	trap_flag[number] = 0;
    if (((*pui32value == alarmVal1)||(*pui32value == alarmVal2)) && (trap_flag[number] == 0))
    {
        trap_flag[number] = 1;
    }
    if ((trap_flag[number] == 1))
    {
        switch(number)
        {
            // ================ Main Specs ================//
            // Send Trap 1: Fire Alarm
            // Send Trap 2: Smoke Alarm
            // Send Trap 3: MotionDetect Alarm
            // Send Trap 4: FloodDetect Alarm
            // Send Trap 5: DoorOpen Alarm
            // Send Trap 6: GenFailure Alarm
            // Send Trap 7: DcThres Alarm
            // Send Trap 8: GenStop Alarm
            // Send Trap 9: AcThres Alarm
            // Send Trap 10: 
            // Send Trap 11 .. 18: DigitalInputAlarm1 .. 8
            // Send Trap 19: 
        case 1:
        case 2:  
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:    
            {
                //Add the battery2ThresVolt.0 object to the variable binding list of the message
                oidFromString(str,oid,maxOidLen, oidLen);
                
                //Send a SNMP trap
                snmpAgentSendTrap(context, destIpAddr, version,
                                  username,genericTrapType , specificTrapCode, objectList, objectListSize); //
                
            }break;						
        }
        trap_flag[number] = 2;
        
    } else if ((trap_flag[number] == 2) && (*pui32value == normalVal))
    {
        switch(number)
        { 
        case 1:
        case 2:  
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            {
                //Add the battery2ThresVolt.0 object to the variable binding list of the message
                oidFromString(str,oid,maxOidLen, oidLen);
                
                //Send a SNMP trap
                snmpAgentSendTrap(context, destIpAddr, version,
                                  username,genericTrapType , specificTrapCode, objectList, objectListSize); //
                
            }break;	
        }
        trap_flag[number] = 0;
    }
}

void Trap_Send_Type_2(SnmpAgentContext *context, const IpAddr *destIpAddr,
                      SnmpVersion version, const char_t *username, uint_t genericTrapType,
                      uint_t specificTrapCode, const SnmpTrapObject *objectList, uint_t objectListSize ,
                      uint32_t* pui32value_new, uint32_t* pui32value_old)
{  
    //	trap_flag[number] = 0;
    if (*pui32value_new != *pui32value_old)
    {
        //Send a SNMP trap
        snmpAgentSendTrap(context, destIpAddr, version,
                          username,genericTrapType , specificTrapCode, objectList, objectListSize); 
        *pui32value_old = *pui32value_new;
    }
}