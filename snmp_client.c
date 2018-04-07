#include <stdlib.h>
#include "net_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "snmp/snmp_agent.h"
#include "mibs/mib2_module.h"
#include "mibs/mib2_impl.h"
#include "oid.h"
#include "private_mib_module.h"
#include "private_mib_impl.h"
#include "debug.h"
#include "error.h"
#include "variables.h"
#include "core/net.h"
#include "snmp_client.h"

#if (USERDEF_CLIENT_SNMP == ENABLED)
#define APP_SNMP_ENTERPRISE_OID "1.3.6.1.4.1.45796.1.16"//"1.3.6.1.4.1.8072.9999.9998"//
#define APP_SNMP_CONTEXT_ENGINE "\x80\x00\x00\x00\x01\x02\x03\x04"
#define APP_SNMP_TRAP_DEST_IP_ADDR "192.168.100.25"//"117.6.55.97"//
SnmpAgentSettings snmpAgentSettings;
SnmpAgentContext snmpAgentContext;
size_t oidLen;
uint8_t oid[SNMP_MAX_OID_SIZE];  
#endif //(USERDEF_CLIENT_SNMP == ENABLED)

/*******************************************************************************
* Prototypes
******************************************************************************/
#if (USERDEF_CLIENT_SNMP == ENABLED)
/**
* @brief Random data generation callback function
* @param[out] data Buffer where to store the random data
* @param[in] lenght Number of bytes that are required
* @return Error code
**/

static error_t snmpAgentRandCallback(uint8_t *data, size_t length)
{
    size_t i;
    
    //Generate some random data
    for(i = 0; i < length; i++)
        data[i] = rand();
    
    //No error to report
    return NO_ERROR;
}


static void SnmpSendTrapType1(SnmpAgentContext *context, const IpAddr *destIpAddr,
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

static void SnmpSendTrapType2(SnmpAgentContext *context, const IpAddr *destIpAddr,
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

static void SnmpSendAlarmTrap(SnmpTrapObject* trapObjects, IpAddr destIpAddr)
{
  //Add the alarmSmokeAlarms.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.15.1.0", trapObjects[0].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
  //Add the siteInfoBTSCode.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
  SnmpSendTrapType2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                    "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,1, trapObjects, 2,                             
                    &privateMibBase.alarmGroup.alarmFireAlarms, 
                    &privateMibBase.alarmGroup.alarmFireAlarms_old);
  
  //Add the alarmSmokeAlarms.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.15.2.0", trapObjects[1].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
  //Add the siteInfoBTSCode.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[0].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
  SnmpSendTrapType2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                    "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,2, trapObjects, 2,                              
                    &privateMibBase.alarmGroup.alarmSmokeAlarms, 
                    &privateMibBase.alarmGroup.alarmSmokeAlarms_old);
  
  //Add the alarmMotionDetectAlarms.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.15.3.0", trapObjects[0].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen); 
  //Add the siteInfoBTSCode.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);       
  SnmpSendTrapType2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                    "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,3, trapObjects, 2,                             
                    &privateMibBase.alarmGroup.alarmMotionDetectAlarms, 
                    &privateMibBase.alarmGroup.alarmMotionDetectAlarms_old);
  
  //Add the alarmFloodDetectAlarms.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.15.4.0", trapObjects[0].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
  //Add the siteInfoBTSCode.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
  SnmpSendTrapType2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                    "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,4, trapObjects, 2,                             
                    &privateMibBase.alarmGroup.alarmFloodDetectAlarms, 
                    &privateMibBase.alarmGroup.alarmFloodDetectAlarms_old);
  
  //Add the alarmDoorOpenAlarms.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.15.5.0", trapObjects[0].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
  //Add the siteInfoBTSCode.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
  SnmpSendTrapType2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                    "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,5, trapObjects, 2,
                    &privateMibBase.alarmGroup.alarmDoorOpenAlarms, 
                    &privateMibBase.alarmGroup.alarmDoorOpenAlarms_old);
  
  //Add the alarmGenFailureAlarms.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.15.6.0", trapObjects[0].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
  //Add the siteInfoBTSCode.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen); 
  SnmpSendTrapType2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                    "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,6, trapObjects, 2,
                    &privateMibBase.alarmGroup.alarmGenFailureAlarms, 
                    &privateMibBase.alarmGroup.alarmGenFailureAlarms_old);
  
  //Add the alarmDcThresAlarms.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.15.7.0", trapObjects[0].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
  //Add the siteInfoBTSCode.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);  
  SnmpSendTrapType2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                    "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,7, trapObjects, 2,
                    &privateMibBase.alarmGroup.alarmDcThresAlarms, 
                    &privateMibBase.alarmGroup.alarmDcThresAlarms_old);
  
  //Add the alarmMachineStopAlarms.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.15.8.0", trapObjects[0].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
  //Add the siteInfoBTSCode.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);  
  SnmpSendTrapType2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                    "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,8, trapObjects, 2,
                    &privateMibBase.alarmGroup.alarmMachineStopAlarms, 
                    &privateMibBase.alarmGroup.alarmMachineStopAlarms_old);
  
  //Add the alarmAccessAlarms.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.15.9.0", trapObjects[0].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
  //Add the siteInfoBTSCode.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);  
  SnmpSendTrapType2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                    "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,9, trapObjects, 2,
                    &privateMibBase.alarmGroup.alarmAccessAlarms, 
                    &privateMibBase.alarmGroup.alarmAccessAlarms_old);
  
  //Add the alarmAcThresAlarms.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.15.10.0", trapObjects[0].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);  
  //Add the siteInfoBTSCode.0 object to the variable binding list of the message
  oidFromString("1.3.6.1.4.1.45796.1.1.1.0", trapObjects[1].oid,
                SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);  
  SnmpSendTrapType2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                    "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,9, trapObjects, 2,
                    &privateMibBase.alarmGroup.alarmAccessAlarms, 
                    &privateMibBase.alarmGroup.alarmAccessAlarms_old);
  
}

static void SnmpSendSiteInfoTrap(SnmpTrapObject* trapObjects, IpAddr destIpAddr)
{
  error_t error;
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
}

static void SnmpSendAcInfoTrap(SnmpTrapObject* trapObjects, IpAddr destIpAddr)
{
  error_t error;
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
}

static void SnmpSendBatteryInfoTrap(SnmpTrapObject* trapObjects, IpAddr destIpAddr)
{
  error_t error;
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
}

static void SnmpSendAccessoriesInfoTrap(SnmpTrapObject* trapObjects, IpAddr destIpAddr)
{
  error_t error;
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
}

static void SnmpSendConfigurationInfoTrap(SnmpTrapObject* trapObjects, IpAddr destIpAddr)
{
  error_t error;
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
}

static void SnmpSendAlarmInfoTrap(SnmpTrapObject* trapObjects, IpAddr destIpAddr)
{
  error_t error;
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
  //        osDelayTask(100);
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

static void SnmpSendTrap(void)
{
    IpAddr destIpAddr;
    SnmpTrapObject trapObjects[65];
    //Destination IP address
    ipStringToAddr((const char_t*)sMenu_Variable.ucSIP, &destIpAddr);  
    SnmpSendAlarmTrap(trapObjects, destIpAddr);
    /*
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
    */
    if (trapStatus_TimePeriod >= 30)
    {        
        SnmpSendSiteInfoTrap(trapObjects, destIpAddr);
        SnmpSendAcInfoTrap(trapObjects, destIpAddr);
        SnmpSendBatteryInfoTrap(trapObjects, destIpAddr);
        SnmpSendAccessoriesInfoTrap(trapObjects, destIpAddr);
	SnmpSendConfigurationInfoTrap(trapObjects, destIpAddr);
        SnmpSendAlarmInfoTrap(trapObjects, destIpAddr);
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

void SnmpSendTrapTask(void *param)
{
  
  //Endless loop
  while(1)
  {
    SnmpSendTrap();
    osDelayTask(3000);
  }
}

void SnmpInitMib()
{
  error_t error;
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
}

error_t SnmpInitClient(NetInterface* interface)
{
  error_t error;
  // if interface = NULL then can not iit snmpAgent --> print error and while
  if (interface == NULL)
  {
    TRACE_ERROR("network interface is not valid - can not initialize snmp agent");
    while(1);
  }    
  snmpAgentGetDefaultSettings(&snmpAgentSettings);
  snmpAgentSettings.interface = interface;
  snmpAgentSettings.versionMin = SNMP_VERSION_1;
  snmpAgentSettings.versionMax = SNMP_VERSION_2C;
  
#if (SNMP_V3_SUPPORT == ENABLED)
  snmpAgentSettings.versionMax = SNMP_VERSION_3;
  snmpAgentSettings.randCallback = snmpAgentRandCallback;
#endif //(SNMP_V3_SUPPORT == ENABLED)
  
  //SNMP agent initialization
  error = snmpAgentInit(&snmpAgentContext, &snmpAgentSettings);
  if(error)
  {
    //Debug message
    TRACE_ERROR("Failed to initialize SNMP agent!\r\n");
    return error;
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
#endif //(SNMP_V3_SUPPORT == ENABLED)
  
  //Start SNMP agent
  error = snmpAgentStart(&snmpAgentContext);
  //Failed to start SNMP agent?
  if(error)
  {
    //Debug message
    TRACE_ERROR("Failed to start SNMP agent!\r\n");
  }
  return error;
}
#endif //(USERDEF_CLIENT_SNMP == ENABLED)