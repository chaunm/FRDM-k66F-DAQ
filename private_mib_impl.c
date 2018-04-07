/**
* @file private_mib_impl.c
* @brief Private MIB module implementation
*
* @section License
*
* ^^(^____^)^^
**/

//Dependencies
#include "mk66f18.h"
#include "frdm_k66f.h"
#include "core/net.h"
#include "private_mib_module.h"
#include "private_mib_impl.h"
#include "crypto.h"
#include "asn1.h"
#include "oid.h"
#include "debug.h"
#include "variables.h"
#include "access_control.h"

uint32_t setCount_test;
//Mutex preventing simultaneous access to the private MIB base
static OsMutex privateMibMutex;


/**
* @brief Private MIB module initialization
* @return Error code
**/

error_t privateMibInit(void)
{
  //Debug message
  TRACE_INFO("Initializing private MIB base...\r\n");
  
  //Clear private MIB base
  memset(&privateMibBase, 0, sizeof(privateMibBase));
  
  //Default value for testString object
  strcpy(privateMibBase.testString, "Hello World!");
  privateMibBase.testStringLen = strlen(privateMibBase.testString);
  
  //Default value for testInteger object
  privateMibBase.testInteger = 123;
  
  //Number of LEDs
  privateMibBase.ledCount = PRIVATE_MIB_LED_COUNT;
  
  //LED1 color
  strcpy(privateMibBase.ledTable[0].ledColor, "green");
  privateMibBase.ledTable[0].ledColorLen = strlen(privateMibBase.ledTable[0].ledColor);
  //LED1 state
  privateMibBase.ledTable[0].ledState = 0;
  
  //LED2 color
  strcpy(privateMibBase.ledTable[1].ledColor, "orange");
  privateMibBase.ledTable[1].ledColorLen = strlen(privateMibBase.ledTable[1].ledColor);
  //LED2 state
  privateMibBase.ledTable[1].ledState = 0;
  
  //LED3 color
  strcpy(privateMibBase.ledTable[2].ledColor, "red");
  privateMibBase.ledTable[2].ledColorLen = strlen(privateMibBase.ledTable[2].ledColor);
  //LED3 state
  privateMibBase.ledTable[2].ledState = 0;
  
  //Default value for testString object
  strcpy(privateMibBase.siteInfoGroup.siteInfoBTSCode, "OnlineCETSite");
  privateMibBase.siteInfoGroup.siteInfoBTSCodeLen = strlen(privateMibBase.siteInfoGroup.siteInfoBTSCode);       
  
  privateMibBase.siteInfoGroup.siteInfoThresTemp1 = 50;
  privateMibBase.siteInfoGroup.siteInfoThresTemp2 = 100;
  privateMibBase.siteInfoGroup.siteInfoThresTemp3 = 500;
  privateMibBase.siteInfoGroup.siteInfoThresTemp4 = 700;
  privateMibBase.siteInfoGroup.siteInfoMeasuredTemp = 274;
  privateMibBase.siteInfoGroup.siteInfoMeasuredHumid = 80;
  
  privateMibBase.acPhaseGroup.acPhaseNumber = 1;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseVolt = 222;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseCurrent = 405;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhasePower = 8891;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseIndex = 1;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseFrequency = 600;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseThresVolt = 150;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseAlarmStatus = 0;
  
  privateMibBase.configGroup.configAccNumber = 2;
  strcpy(privateMibBase.configGroup.configAccTable[0].configAccName, "usr-md5-aes");
  privateMibBase.configGroup.configAccTable[0].configAccNameLen = strlen(privateMibBase.configGroup.configAccTable[0].configAccName);
  strcpy(privateMibBase.configGroup.configAccTable[0].configAccAuthkey, "authkey2");
  privateMibBase.configGroup.configAccTable[0].configAccAuthkeyLen = strlen(privateMibBase.configGroup.configAccTable[0].configAccAuthkey);
  strcpy(privateMibBase.configGroup.configAccTable[0].configAccPrivkey, "privkey2");
  privateMibBase.configGroup.configAccTable[0].configAccPrivkeyLen = strlen(privateMibBase.configGroup.configAccTable[0].configAccPrivkey);
  
  privateMibBase.configGroup.configAccessIdNumber = 5;
  strcpy(privateMibBase.configGroup.configAccessIdTable[0].configAccessIdCard, "CARDIDLESON1990");
  privateMibBase.configGroup.configAccessIdTable[0].configAccessIdCardLen = strlen(privateMibBase.configGroup.configAccessIdTable[0].configAccessIdCard);
  //Create a mutex to prevent simultaneous access to the private MIB base
  if(!osCreateMutex(&privateMibMutex))
  {
    //Failed to create mutex
    return ERROR_OUT_OF_RESOURCES;
  }
  
  //Successful processing
  return NO_ERROR;
}


/**
* @brief Lock private MIB base
**/

void privateMibLock(void)
{
  //Enter critical section
  osAcquireMutex(&privateMibMutex);
}


/**
* @brief Unlock private MIB base
**/

void privateMibUnlock(void)
{
  //Leave critical section
  osReleaseMutex(&privateMibMutex);
}


/**
* @brief Get currentTime object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibGetCurrentTime(const MibObject *object, const uint8_t *oid,
                                 size_t oidLen, MibVariant *value, size_t *valueLen)
{
  //Get object value
  value->timeTicks = osGetSystemTime() / 10;
  //Successful processing
  return NO_ERROR;
}

error_t privateMibSetStringEntry(const MibObject *object, const uint8_t *oid,
                                 size_t oidLen, const MibVariant *value, size_t valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  char_t *entry;
  
  //Point to the end of the OID prefix
  n = object->oidLen;
  
  //Get the instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Point to the LED table entry
  entry = &privateMibBase.testString[0];
  
  //ledState object?
  if(!strcmp(object->name, "testString"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(valueLen >= sizeof(privateMibBase.testString))
      return ERROR_BUFFER_OVERFLOW;
    
    //Copy object value
    memset(entry,0,sizeof(privateMibBase.testString));
    memcpy(entry, value->octetString, valueLen);	 
    //Debug message
    TRACE_ERROR("New testString:%s!\r\n",&privateMibBase.testString[0]);		 
  }
  //Unknown object?
  else
  {
    TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR;
}

/**
* @brief Set ledEntry object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[in] value Object value
* @param[in] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibSetLedEntry(const MibObject *object, const uint8_t *oid,
                              size_t oidLen, const MibVariant *value, size_t valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibLedEntry *entry;
  
  //Point to the end of the OID prefix
  n = object->oidLen;
  
  //Get the instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Check index range
  if(index < 1 || index > PRIVATE_MIB_LED_COUNT)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the LED table entry
  entry = &privateMibBase.ledTable[index - 1];
  
  //ledState object?
  if(!strcmp(object->name, "ledState"))
  {
    //Set object value
    entry->ledState = value->integer;
    
    //Update LED state
    if(index == 1)
    {
      if(entry->ledState)
        GPIO_LED_R->PCOR = LED_R_MASK;
      else
        GPIO_LED_R->PSOR = LED_R_MASK;
    }
    else if(index == 2)
    {
      if(entry->ledState)
        GPIO_LED_G->PCOR = LED_G_MASK;
      else
        GPIO_LED_G->PSOR = LED_G_MASK;
    }
    else if(index == 3)
    {
      if(entry->ledState)
        GPIO_LED_B->PCOR = LED_B_MASK;
      else
        GPIO_LED_B->PSOR = LED_B_MASK;
    }
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR;
}


/**
* @brief Get ledEntry object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibGetLedEntry(const MibObject *object, const uint8_t *oid,
                              size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibLedEntry *entry;
  
  //Point to the end of the OID prefix
  n = object->oidLen;
  
  //Get the instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Check index range
  if(index < 1 || index > PRIVATE_MIB_LED_COUNT)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the LED table entry
  entry = &privateMibBase.ledTable[index - 1];
  
  //ledColor object?
  if(!strcmp(object->name, "ledColor"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen < entry->ledColorLen)
      return ERROR_BUFFER_OVERFLOW;
    
    //Copy object value
    memcpy(value->octetString, entry->ledColor, entry->ledColorLen);
    //Return object length
    *valueLen = entry->ledColorLen;
  }
  //ledState object?
  else if(!strcmp(object->name, "ledState"))
  {
    //Get object value
    value->integer = entry->ledState;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR;
}


/**
* @brief Get next ledEntry object
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier
* @param[in] oidLen Length of the OID, in bytes
* @param[out] nextOid OID of the next object in the MIB
* @param[out] nextOidLen Length of the next object identifier, in bytes
* @return Error code
**/

error_t privateMibGetNextLedEntry(const MibObject *object, const uint8_t *oid,
                                  size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through LED table
  for(index = 1; index <= PRIVATE_MIB_LED_COUNT; index++)
  {
    //Point to the end of the OID prefix
    n = object->oidLen;
    
    //Append the instance identifier to the OID prefix
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}
//========================================== SiteInfo Function ==========================================//

error_t privateMibSetSiteInfoGroup(const MibObject *object, const uint8_t *oid,
                                   size_t oidLen, const MibVariant *value, size_t valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibSiteInfoGroup *entry;
  
  //Point to the end of the OID prefix
  n = object->oidLen;
  
  //   //Get the instance identifier
  //   error = mibDecodeIndex(oid, oidLen, &n, &index);
  //   //Invalid instance identifier?
  //   if(error) return error;
  //
  //   //Check index range
  //   if(index < 1 || index > 8)
  //      return ERROR_INSTANCE_NOT_FOUND;   
  
  //	Point to the siteInfoGroup entry
  entry = &privateMibBase.siteInfoGroup;
  
  //siteInfoBTSCode object?
  if(!strcmp(object->name, "siteInfoBTSCode"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(valueLen >= sizeof(privateMibBase.siteInfoGroup.siteInfoBTSCode))
      return ERROR_BUFFER_OVERFLOW;
    
    //Copy object value
    memset(entry->siteInfoBTSCode,0,sizeof(privateMibBase.siteInfoGroup.siteInfoBTSCode));
    entry->siteInfoBTSCodeLen = valueLen;
    //Debug message
  }
  //siteInfoThresTemp1 object?
  else if(!strcmp(object->name, "siteInfoThresTemp1"))
  {
    //Get object value
    entry->siteInfoThresTemp1 = value->integer;                  
    sMenu_Variable.u16ThresTemp[0] = entry->siteInfoThresTemp1; 
    WriteEEPROM_Word(sSetting_Values[_TEMP1].addrEEPROM,sMenu_Variable.u16ThresTemp[0]); 		              	
  }
  //siteInfoThresTemp2 object?
  else if(!strcmp(object->name, "siteInfoThresTemp2"))
  {
    //Get object value
    entry->siteInfoThresTemp2 = value->integer;
    sMenu_Variable.u16ThresTemp[1] = entry->siteInfoThresTemp2; 
    WriteEEPROM_Word(sSetting_Values[_TEMP2].addrEEPROM,sMenu_Variable.u16ThresTemp[1]); 			
  }
  //siteInfoThresTemp3 object?
  else if(!strcmp(object->name, "siteInfoThresTemp3"))
  {        
    //Get object value
    entry->siteInfoThresTemp3= value->integer;
    sMenu_Variable.u16ThresTemp[2] = entry->siteInfoThresTemp3; 
    WriteEEPROM_Word(sSetting_Values[_TEMP3].addrEEPROM,sMenu_Variable.u16ThresTemp[2]); 		 		
  }
  //siteInfoThresTemp4 object?
  else if(!strcmp(object->name, "siteInfoThresTemp4"))
  {
    //Get object value
    entry->siteInfoThresTemp4= value->integer;
    sMenu_Variable.u16ThresTemp[3] = entry->siteInfoThresTemp4; 
    WriteEEPROM_Word(sSetting_Values[_TEMP4].addrEEPROM,sMenu_Variable.u16ThresTemp[3]); 
  }
  //Unknown object?
  else
  {
    TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR;
}

/**
* @brief Get siteInfo object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibGetSiteInfoGroup(const MibObject *object, const uint8_t *oid,
                                   size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  size_t n;
  //	uint_t index;
  //	Ipv4Addr ipAddr;
  //	NetInterface *interface;
  PrivateMibSiteInfoGroup *entry;
  //	interface = &netInterface[0];
  //Point to the instance identifier
  //	n = object->oidLen;
  //
  //	//The ipNetToMediaIfIndex is used as 1st instance identifier
  //	error = mibDecodeIndex(oid, oidLen, &n, &index);
  //	//Invalid instance identifier?
  //	if(error) return error;
  //
  //	//The ipNetToMediaNetAddress is used as 2nd instance identifier
  //	error = mibDecodeIpv4Addr(oid, oidLen, &n, &ipAddr);
  //	//Invalid instance identifier?
  //	if(error) return error;
  //
  //	//Sanity check
  //	if(n != oidLen)
  //	return ERROR_INSTANCE_NOT_FOUND;
  //	Point to the siteInfoGroup entry
  entry = &privateMibBase.siteInfoGroup;
  
  //siteInfoBTSCode object?
  if(!strcmp(object->name, "siteInfoBTSCode"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->siteInfoBTSCodeLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->siteInfoBTSCode, entry->siteInfoBTSCodeLen);
      //Return object length
      *valueLen = entry->siteInfoBTSCodeLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //siteInfoThresTemp1 object?
  else if(!strcmp(object->name, "siteInfoThresTemp1"))
  {
    //Get object value
    value->integer = privateMibBase.siteInfoGroup.siteInfoThresTemp1;
  }
  //siteInfoThresTemp2 object?
  else if(!strcmp(object->name, "siteInfoThresTemp2"))
  {
    //Get object value
    value->integer = privateMibBase.siteInfoGroup.siteInfoThresTemp2;
  }
  //siteInfoThresTemp3 object?
  else if(!strcmp(object->name, "siteInfoThresTemp3"))
  {
    //Get object value
    value->integer = privateMibBase.siteInfoGroup.siteInfoThresTemp3;
  }
  //siteInfoThresTemp4 object?
  else if(!strcmp(object->name, "siteInfoThresTemp4"))
  {
    //Get object value
    value->integer = privateMibBase.siteInfoGroup.siteInfoThresTemp4;
  }
  //siteInfoMeasuredTemp object?
  else if(!strcmp(object->name, "siteInfoMeasuredTemp"))
  {
    //Get object value
    value->integer = privateMibBase.siteInfoGroup.siteInfoMeasuredTemp;
  }
  //siteInfoMeasuredHumid object?
  else if(!strcmp(object->name, "siteInfoMeasuredHumid"))
  {
    //Get object value
    value->integer = privateMibBase.siteInfoGroup.siteInfoMeasuredHumid;
  }
  //siteInfoAccessId object?
  else if(!strcmp(object->name, "siteInfoAccessId"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->siteInfoAccessIdLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->siteInfoAccessId, entry->siteInfoAccessIdLen);
      //Return object length
      *valueLen = entry->siteInfoAccessIdLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //siteInfoTrapCounter object?
  else if(!strcmp(object->name, "siteInfoTrapCounter"))
  {
    //Get object value
    value->integer = privateMibBase.siteInfoGroup.siteInfoTrapCounter;
  }
  // siteInfoIpAddress
  else if(!strcmp(object->name, "siteInfoIpAddress"))
  {
    //Get object value
    value->integer = privateMibBase.siteInfoGroup.siteInfoIpAddress;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  //siteInforIpAddress

  
  //Return status code
  return error;
}

//========================================== SiteInfo Function ==========================================//
//========================================== AcInfo Function ==========================================//
/**
* @brief Set acInfoEntry object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/
error_t privateMibSetAcPhaseEntry(const MibObject *object, const uint8_t *oid,
                                  size_t oidLen, const MibVariant *value, size_t valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibAcPhaseEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 3)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the interface table entry
  entry = &privateMibBase.acPhaseGroup.acPhaseTable[index - 1];// &mib2Base.ifGroup.ifTable[index - 1];
  
  //ifIndex object?
  if(!strcmp(object->name, "acPhaseThresVolt"))
  {
    //Get object value
    entry->acPhaseThresVolt = value->integer;
    sMenu_Variable.u16AcThresVolt[index - 1] = entry->acPhaseThresVolt; 
    WriteEEPROM_Word(sSetting_Values[_AC_LOW].addrEEPROM,sMenu_Variable.u16AcThresVolt[index - 1]);         
  }
  
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}
/**
* @brief Get acInfoEntry object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibGetAcPhaseEntry(const MibObject *object, const uint8_t *oid,
                                  size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibAcPhaseEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 3)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the interface table entry
  entry = &privateMibBase.acPhaseGroup.acPhaseTable[index - 1];// &mib2Base.ifGroup.ifTable[index - 1];
  
  //ifIndex object?
  if(!strcmp(object->name, "acPhaseIndex"))
  {
    //Get object value
    value->integer = entry->acPhaseIndex;
  }
  //acPhaseVolt object?
  else if(!strcmp(object->name, "acPhaseVolt"))
  {
    //Get object value
    value->integer = entry->acPhaseVolt;
  }
  //acPhaseAlarmStatus object?
  else if(!strcmp(object->name, "acPhaseAlarmStatus"))
  {
    //Get object value
    value->integer = entry->acPhaseAlarmStatus;
  }
  //acPhaseCurrent object?
  else if(!strcmp(object->name, "acPhaseCurrent"))
  {
    //Get object value
    value->integer = entry->acPhaseCurrent;
  }
  //acPhasePower object?
  else if(!strcmp(object->name, "acPhasePower"))
  {
    //Get object value
    value->integer = entry->acPhasePower;
  }
  //acPhaseFrequency object?
  else if(!strcmp(object->name, "acPhaseFrequency"))
  {
    //Get object value
    value->integer = entry->acPhaseFrequency;
  }
  //acPhaseThresVolt object?
  else if(!strcmp(object->name, "acPhaseThresVolt"))
  {
    //Get object value
    value->integer = entry->acPhaseThresVolt;
  }
  
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}


/**
* @brief Get next acInfoEntry object
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier
* @param[in] oidLen Length of the OID, in bytes
* @param[out] nextOid OID of the next object in the MIB
* @param[out] nextOidLen Length of the next object identifier, in bytes
* @return Error code
**/
error_t privateMibGetNextAcPhaseEntry(const MibObject *object, const uint8_t *oid,
                                      size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= privateMibBase.acPhaseGroup.acPhaseNumber; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}
//========================================== AcInfo Function ==========================================//
//========================================== BatteryInfo Function ==========================================//
/**
* @brief Set BatteryInfo object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
**/
error_t privateMibSetBatteryGroup(const MibObject *object, const uint8_t *oid,
                                  size_t oidLen, const MibVariant *value, size_t valueLen)
{
  error_t error = NO_ERROR;
  size_t n;
  PrivateMibBatteryGroup *entry;
  
  //Point to the end of the OID prefix
  n = object->oidLen;
  
  //	Point to the siteInfoGroup entry
  entry = &privateMibBase.batteryGroup;
  
  //battery1ThresVolt object?
  if(!strcmp(object->name, "battery1ThresVolt"))
  {
    //Get object value
    entry->battery1ThresVolt = value->integer; 
    sMenu_Variable.u16BattThresVolt[0] = entry->battery1ThresVolt; 
    WriteEEPROM_Word(sSetting_Values[_DC_LOW].addrEEPROM,sMenu_Variable.u16BattThresVolt[0]);                 		              	
  }
  //battery2ThresVolt object?
  else if(!strcmp(object->name, "battery2ThresVolt"))
  {
    //Get object value
    entry->battery2ThresVolt = value->integer;			
  }
  //Unknown object?
  else
  {
    TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR;
}

/**
* @brief Get BatteryInfo object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibGetBatteryGroup(const MibObject *object, const uint8_t *oid,
                                  size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  size_t n;
  //	uint_t index;
  //	Ipv4Addr ipAddr;
  //	NetInterface *interface;
  PrivateMibBatteryGroup *entry;
  //	interface = &netInterface[0];
  //Point to the instance identifier
  //	n = object->oidLen;
  //
  //	//The ipNetToMediaIfIndex is used as 1st instance identifier
  //	error = mibDecodeIndex(oid, oidLen, &n, &index);
  //	//Invalid instance identifier?
  //	if(error) return error;
  //
  //	//The ipNetToMediaNetAddress is used as 2nd instance identifier
  //	error = mibDecodeIpv4Addr(oid, oidLen, &n, &ipAddr);
  //	//Invalid instance identifier?
  //	if(error) return error;
  //
  //	//Sanity check
  //	if(n != oidLen)
  //	return ERROR_INSTANCE_NOT_FOUND;
  
  //battery1Voltage object?
  if(!strcmp(object->name, "battery1Voltage"))
  {
    //Get object value
    value->integer = privateMibBase.batteryGroup.battery1Voltage;
  }
  //battery2Voltage object?
  else if(!strcmp(object->name, "battery2Voltage"))
  {
    //Get object value
    value->integer = privateMibBase.batteryGroup.battery2Voltage;
  }
  //battery1AlarmStatus object?
  else if(!strcmp(object->name, "battery1AlarmStatus"))
  {
    //Get object value
    value->integer = privateMibBase.batteryGroup.battery1AlarmStatus;
  }
  //battery2AlarmStatus object?
  else if(!strcmp(object->name, "battery2AlarmStatus"))
  {
    //Get object value
    value->integer = privateMibBase.batteryGroup.battery2AlarmStatus;
  }
  //battery1ThresVolt object?
  else if(!strcmp(object->name, "battery1ThresVolt"))
  {
    //Get object value
    value->integer = privateMibBase.batteryGroup.battery1ThresVolt;
  }
  //battery2ThresVolt object?
  else if(!strcmp(object->name, "battery2ThresVolt"))
  {
    //Get object value
    value->integer = privateMibBase.batteryGroup.battery2ThresVolt;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}
//========================================== BatteryInfo Function ==========================================//
//========================================== AccessoriesInfo Function ==========================================//
/**
* @brief Set AccessoriesInfo object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/
error_t privateMibSetAccessoriesGroup(const MibObject *object, const uint8_t *oid,
                                      size_t oidLen, const MibVariant *value, size_t valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibAccessoriesGroup *entry;
  
  //Point to the end of the OID prefix
  n = object->oidLen;
  
  //	Point to the siteInfoGroup entry
  entry = &privateMibBase.accessoriesGroup;
  
  //siteInfoBTSCode object?
  if(!strcmp(object->name, "doorStatus"))
  {
    //Get object value
    entry->doorStatus = value->integer; 
    if (entry->doorStatus == 1)
    {
      IO_OPENDOOR_MCU_ON(); 
    } 
    else if (entry->doorStatus == 0)
    {
      IO_OPENDOOR_MCU_OFF(); 
    }
  }
  //siteInfoThresTemp1 object?
  else if(!strcmp(object->name, "airConSetTemp1"))
  {
    //Get object value
    entry->airConSetTemp1 = value->integer;                  
    sMenu_Variable.changedVal = _AIRCON_TEMP1; 
    sMenu_Variable.u16AirConTemp[0] = entry->airConSetTemp1; 
    WriteEEPROM_Word(sSetting_Values[_AIRCON_TEMP1].addrEEPROM,sMenu_Variable.u16AirConTemp[0]); 			              	
  }
  //siteInfoThresTemp2 object?
  else if(!strcmp(object->name, "airConSetTemp2"))
  {
    //Get object value
    entry->airConSetTemp2 = value->integer;
    sMenu_Variable.changedVal = _AIRCON_TEMP2; 
    sMenu_Variable.u16AirConTemp[1] = entry->airConSetTemp2; 
    WriteEEPROM_Word(sSetting_Values[_AIRCON_TEMP2].addrEEPROM,sMenu_Variable.u16AirConTemp[1]); 				
  }
  //siteInfoThresTemp3 object?
  else if(!strcmp(object->name, "airConSetTemp3"))
  {        
    //Get object value
    entry->airConSetTemp3= value->integer;
    sMenu_Variable.changedVal = _AIRCON_TEMP3; 
    sMenu_Variable.u16AirConTemp[2] = entry->airConSetTemp3; 
    WriteEEPROM_Word(sSetting_Values[_AIRCON_TEMP3].addrEEPROM,sMenu_Variable.u16AirConTemp[2]); 				 		
  }
  //siteInfoThresTemp4 object?
  else if(!strcmp(object->name, "airConSetTemp4"))
  {
    //Get object value
    entry->airConSetTemp4= value->integer;
    sMenu_Variable.changedVal = _AIRCON_TEMP4; 
    sMenu_Variable.u16AirConTemp[3] = entry->airConSetTemp4; 
    WriteEEPROM_Word(sSetting_Values[_AIRCON_TEMP4].addrEEPROM,sMenu_Variable.u16AirConTemp[3]);
  }
  //ledControlStatus object?
  else if(!strcmp(object->name, "ledControlStatus"))
  {
    //Get object value
    entry->ledControlStatus = value->integer; 
  }
  //speakerControlStatus object?
  else if(!strcmp(object->name, "speakerControlStatus"))
  {
    //Get object value
    entry->speakerControlStatus = value->integer; 
  }
  //Unknown object?
  else
  {
    TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR;
}
/**
* @brief Get AccessoriesInfo object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibGetAccessoriesGroup(const MibObject *object, const uint8_t *oid,
                                      size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  size_t n;
  PrivateMibAccessoriesGroup *entry;
  
  entry = &privateMibBase.accessoriesGroup;
  //airCon1Status object?
  if(!strcmp(object->name, "airCon1Status"))
  {
    //Get object value
    value->integer = entry->airCon1Status;
  }
  //airCon2Status object?
  else if(!strcmp(object->name, "airCon2Status"))
  {
    //Get object value
    value->integer = entry->airCon2Status;
  }
  //fan1Status object?
  else if(!strcmp(object->name, "fan1Status"))
  {
    //Get object value
    value->integer = entry->fan1Status;
  }
  //fan2Status object?
  else if(!strcmp(object->name, "fan2Status"))
  {
    //Get object value
    value->integer = entry->fan2Status;
  }
  //doorStatus object?
  else if(!strcmp(object->name, "doorStatus"))
  {
    //Get object value
    value->integer = entry->doorStatus;
  } 
  //airConSetTemp1 object?
  else if(!strcmp(object->name, "airConSetTemp1"))
  {
    //Get object value
    value->integer = entry->airConSetTemp1;
  }
  //airConSetTemp2 object?
  else if(!strcmp(object->name, "airConSetTemp2"))
  {
    //Get object value
    value->integer = entry->airConSetTemp2;
  }
  //airConSetTemp3 object?
  else if(!strcmp(object->name, "airConSetTemp3"))
  {
    //Get object value
    value->integer = entry->airConSetTemp3;
  }
  //airConSetTemp4 object?
  else if(!strcmp(object->name, "airConSetTemp4"))
  {
    //Get object value
    value->integer = entry->airConSetTemp4;
  }
  //ledControlStatus object?
  else if(!strcmp(object->name, "ledControlStatus"))
  {
    //Get object value
    value->integer = entry->ledControlStatus;
  } 
  //speakerControlStatus object?
  else if(!strcmp(object->name, "speakerControlStatus"))
  {
    //Get object value
    value->integer = entry->speakerControlStatus;
  } 
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}
//========================================== AccessoriesInfo Function ==========================================//
//========================================== ConfigAcc Function ==========================================//
/**
* @brief Get configAccEntry object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibGetConfigAccEntry(const MibObject *object, const uint8_t *oid,
                                    size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibConfigAccEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 5)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the configAcc table entry
  entry = &privateMibBase.configGroup.configAccTable[index - 1];// &mib2Base.ifGroup.ifTable[index - 1];
  
  //configAccIndex object?
  if(!strcmp(object->name, "configAccIndex"))
  {
    //Get object value
    value->integer = entry->configAccIndex;
  }
  //configAccName object?
  else if(!strcmp(object->name, "configAccName"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->configAccNameLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->configAccName, entry->configAccNameLen);
      //Return object length
      *valueLen = entry->configAccNameLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //configAccAuthkey object?
  else if(!strcmp(object->name, "configAccAuthkey"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->configAccAuthkeyLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->configAccAuthkey, entry->configAccAuthkeyLen);
      //Return object length
      *valueLen = entry->configAccAuthkeyLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //configAccPrivkey object?
  else if(!strcmp(object->name, "configAccPrivkey"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->configAccPrivkeyLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->configAccPrivkey, entry->configAccPrivkeyLen);
      //Return object length
      *valueLen = entry->configAccPrivkeyLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //configAccAuthType object?
  else if(!strcmp(object->name, "configAccAuthType"))
  {
    //Get object value
    value->integer = entry->configAccAuthType;
  }
  //configAccPrivType object?
  else if(!strcmp(object->name, "configAccPrivType"))
  {
    //Get object value
    value->integer = entry->configAccPrivType;
  }
  
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}


/**
* @brief Get next configAccEntry object
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier
* @param[in] oidLen Length of the OID, in bytes
* @param[out] nextOid OID of the next object in the MIB
* @param[out] nextOidLen Length of the next object identifier, in bytes
* @return Error code
**/
error_t privateMibGetNextConfigAccEntry(const MibObject *object, const uint8_t *oid,
                                        size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= privateMibBase.configGroup.configAccNumber; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}
//========================================== ConfigAcc Function ==========================================//
//========================================== ConfigAccessId Function ==========================================//
/**
* @brief Set privateMibSetConfigAccessIdEntry object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/
error_t privateMibSetConfigAccessIdEntry(const MibObject *object, const uint8_t *oid,
                                         size_t oidLen, const MibVariant *value, size_t valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibConfigAccessIdEntry *entry;
  uint8_t j;
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 5)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the interface table entry
  entry = &privateMibBase.configGroup.configAccessIdTable[index - 1];// &mib2Base.ifGroup.ifTable[index - 1];
  
  //ifIndex object?
  if(!strcmp(object->name, "configAccessIdCard"))
  {       
    //Make sure the buffer is large enough to hold the entire object
    if(valueLen >= sizeof(entry->configAccessIdCard))
      return ERROR_BUFFER_OVERFLOW;
    
    //Copy object value
    memset(entry->configAccessIdCard,0,sizeof(entry->configAccessIdCard));
    memcpy(entry->configAccessIdCard, value->octetString, valueLen);
    entry->configAccessIdCardLen = 8;
    for(j=0;j<8;j++)
    {
      sMenu_Variable.u8UserID[index-1][j] = entry->configAccessIdCard[j];
    }
    ACS_SaveUserID(sMenu_Variable.u8UserIDAddr[index-1],(uint8_t*)entry->configAccessIdCard);
  }
  
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

/**
* @brief Get configAccessIdEntry object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibGetConfigAccessIdEntry(const MibObject *object, const uint8_t *oid,
                                         size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibConfigAccessIdEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 5)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the configAcc table entry
  entry = &privateMibBase.configGroup.configAccessIdTable[index - 1];// &mib2Base.ifGroup.ifTable[index - 1];
  
  //configAccessIdIndex object?
  if(!strcmp(object->name, "configAccessIdIndex"))
  {
    //Get object value
    value->integer = entry->configAccessIdIndex;
  }
  //configAccessIdCard object?
  else if(!strcmp(object->name, "configAccessIdCard"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->configAccessIdCardLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->configAccessIdCard, entry->configAccessIdCardLen);
      //Return object length
      *valueLen = entry->configAccessIdCardLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}


/**
* @brief Get next configAccessIdEntry object
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier
* @param[in] oidLen Length of the OID, in bytes
* @param[out] nextOid OID of the next object in the MIB
* @param[out] nextOidLen Length of the next object identifier, in bytes
* @return Error code
**/
error_t privateMibGetNextConfigAccessIdEntry(const MibObject *object, const uint8_t *oid,
                                             size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= privateMibBase.configGroup.configAccessIdNumber; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}
//========================================== ConfigAccessId Function ==========================================//
//========================================== ConfigInfo Function ==========================================//
/**
* @brief Get configInfo object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibGetConfigInfoGroup(const MibObject *object, const uint8_t *oid,
                                     size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  size_t n;
  uint_t index;
  Ipv4Addr ipAddr;
  NetInterface *interface;
  ArpCacheEntry *entry;
  interface = &netInterface[0];
  //	//Point to the instance identifier
  //	n = object->oidLen;
  //
  //	//The ipNetToMediaIfIndex is used as 1st instance identifier
  //	error = mibDecodeIndex(oid, oidLen, &n, &index);
  //	//Invalid instance identifier?
  //	if(error) return error;
  //
  //	//The ipNetToMediaNetAddress is used as 2nd instance identifier
  //	error = mibDecodeIpv4Addr(oid, oidLen, &n, &ipAddr);
  //	//Invalid instance identifier?
  //	if(error) return error;
  //
  //	//Sanity check
  //	if(n != oidLen)
  //	return ERROR_INSTANCE_NOT_FOUND;
  
  //configDevIPAddr object?
  if(!strcmp(object->name, "configDevIPAddr"))
  {
    //Get object value
    ipv4CopyAddr(value->ipAddr, &interface->ipv4Context.addr);
  }
  //configDevSNAddr object?
  else if(!strcmp(object->name, "configDevSNAddr"))
  {
    //Get object value
    ipv4CopyAddr(value->ipAddr, &interface->ipv4Context.subnetMask);
  }
  //configDevGWAddr object?
  else if(!strcmp(object->name, "configDevGWAddr"))
  {
    //Get object value
    ipv4CopyAddr(value->ipAddr, &interface->ipv4Context.subnetMask);
  }
  //ipNetToMediaType object?
  else if(!strcmp(object->name, "configDevPort"))
  {
    //Get object value
    value->integer = 161;
  }
  //configServerIPAddr object?
  else if(!strcmp(object->name, "configServerIPAddr"))
  {
    //Get object value
    ipv4CopyAddr(value->ipAddr, &interface->ipv4Context.addr);
  }
  //ipNetToMediaType object?
  else if(!strcmp(object->name, "configServerPort"))
  {
    //Get object value
    value->integer = 162;
  }
  //ipNetToMediaType object?
  else if(!strcmp(object->name, "configRefreshInterval"))
  {
    //Get object value
    value->integer = 60;
  }
  //ipNetToMediaType object?
  else if(!strcmp(object->name, "configUserNumber"))
  {
    //Get object value
    value->integer = 2;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}
//========================================== ConfigInfo Function ==========================================//
//========================================== AlarmInfo Function ==========================================//

/**
* @brief Get alarmInfo object value
* @param[in] object Pointer to the MIB object descriptor
* @param[in] oid Object identifier (object name and instance identifier)
* @param[in] oidLen Length of the OID, in bytes
* @param[out] value Object value
* @param[in,out] valueLen Length of the object value, in bytes
* @return Error code
**/

error_t privateMibGetAlarmGroup(const MibObject *object, const uint8_t *oid,
                                size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  size_t n;
  //	uint_t index;
  //	Ipv4Addr ipAddr;
  //	NetInterface *interface;
  PrivateMibAlarmGroup *entry;
  //	interface = &netInterface[0];
  //Point to the instance identifier
  //	n = object->oidLen;
  //
  //	//The ipNetToMediaIfIndex is used as 1st instance identifier
  //	error = mibDecodeIndex(oid, oidLen, &n, &index);
  //	//Invalid instance identifier?
  //	if(error) return error;
  //
  //	//The ipNetToMediaNetAddress is used as 2nd instance identifier
  //	error = mibDecodeIpv4Addr(oid, oidLen, &n, &ipAddr);
  //	//Invalid instance identifier?
  //	if(error) return error;
  //
  //	//Sanity check
  //	if(n != oidLen)
  //	return ERROR_INSTANCE_NOT_FOUND;
  
  //alarmFireAlarms object?
  if(!strcmp(object->name, "alarmFireAlarms"))
  {
    //Get object value
    value->integer = privateMibBase.alarmGroup.alarmFireAlarms;
  }
  //alarmSmokeAlarms object?
  else if(!strcmp(object->name, "alarmSmokeAlarms"))
  {
    //Get object value
    value->integer = privateMibBase.alarmGroup.alarmSmokeAlarms;
  }
  //alarmMotionDetectAlarms object?
  else if(!strcmp(object->name, "alarmMotionDetectAlarms"))
  {
    //Get object value
    value->integer = privateMibBase.alarmGroup.alarmMotionDetectAlarms;
  }
  //alarmFloodDetectAlarms object?
  else if(!strcmp(object->name, "alarmFloodDetectAlarms"))
  {
    //Get object value
    value->integer = privateMibBase.alarmGroup.alarmFloodDetectAlarms;
  }
  //alarmDoorOpenAlarms object?
  else if(!strcmp(object->name, "alarmDoorOpenAlarms"))
  {
    //Get object value
    value->integer = privateMibBase.alarmGroup.alarmDoorOpenAlarms;
  }
  //alarmGenFailureAlarms object?
  else if(!strcmp(object->name, "alarmGenFailureAlarms"))
  {
    //Get object value
    value->integer = privateMibBase.alarmGroup.alarmGenFailureAlarms;
  }
  //alarmDcThresAlarms object?
  else if(!strcmp(object->name, "alarmDcThresAlarms"))
  {
    //Get object value
    value->integer = privateMibBase.alarmGroup.alarmDcThresAlarms;
  }
  //alarmMachineStopAlarms object?
  else if(!strcmp(object->name, "alarmMachineStopAlarms"))
  {
    //Get object value
    value->integer = privateMibBase.alarmGroup.alarmMachineStopAlarms;
  }
  //alarmAccessAlarms object?
  else if(!strcmp(object->name, "alarmAccessAlarms"))
  {
    //Get object value
    value->integer = privateMibBase.alarmGroup.alarmAccessAlarms & 0xFFFFFF00; // chaunm - the 8th bit for marking
  }
  //alarmAcThresAlarms object?
  else if(!strcmp(object->name, "alarmAcThresAlarms"))
  {
    //Get object value
    value->integer = privateMibBase.alarmGroup.alarmAcThresAlarms;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}
//========================================== AlarmInfo Function ==========================================//


void UpdateInfo (void)
{
  uint8_t i,j;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseVolt = sATS_Variable.vRMS_u32/10;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseCurrent = sATS_Variable.iRMSA_u32/10;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhasePower = sATS_Variable.instan_ActPwA_i32/10;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseIndex = 1;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseFrequency = sATS_Variable.frequency_i16/10;
  privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseThresVolt = sMenu_Variable.u16AcThresVolt[0];
  if( sATS_Variable.vRMS_u32/100 < sMenu_Variable.u16AcThresVolt[0])
  {
    privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseAlarmStatus = 1;
    privateMibBase.alarmGroup.alarmAcThresAlarms = 1;
    sActive_Alarm[0].status = 1;
  } else
  {
    privateMibBase.acPhaseGroup.acPhaseTable[0].acPhaseAlarmStatus = 0;
    privateMibBase.alarmGroup.alarmAcThresAlarms = 0;
    sActive_Alarm[0].status = 0;
  }
  
  privateMibBase.batteryGroup.battery1Voltage = sATS_Variable.battVolt;
  privateMibBase.batteryGroup.battery1AlarmStatus = sATS_Variable.batt_DCLow;
  privateMibBase.batteryGroup.battery1ThresVolt = sMenu_Variable.u16BattThresVolt[0];
  
  if (sATS_Variable.Gen1Fail == 1)
  {
    privateMibBase.alarmGroup.alarmGenFailureAlarms = 1;
    sActive_Alarm[12].status = 1;
  } else
  {
    privateMibBase.alarmGroup.alarmGenFailureAlarms = 0;
    sActive_Alarm[12].status = 0;
  }
  //  if (sATS_Variable.batt_DCLow)
  //  {
  //  privateMibBase.alarmGroup.alarmDcThresAlarms = 1;
  //  sActive_Alarm[1].status = 1;
  //  } else
  //  {
  //  privateMibBase.alarmGroup.alarmDcThresAlarms = 0;
  //  sActive_Alarm[1].status = 0;
  //  }
  if (sATS_Variable.Gen1Voltage == 0) //0:Stop; 1:Run
  {
    privateMibBase.alarmGroup.alarmMachineStopAlarms = 1;
    sActive_Alarm[14].status = 1;
  } else if (sATS_Variable.Gen1Voltage == 1)
  {
    privateMibBase.alarmGroup.alarmMachineStopAlarms = 0;
    sActive_Alarm[14].status = 0;
  } 
  
  privateMibBase.siteInfoGroup.siteInfoThresTemp1 = sMenu_Variable.u16ThresTemp[0];
  privateMibBase.siteInfoGroup.siteInfoThresTemp2 = sMenu_Variable.u16ThresTemp[1];
  privateMibBase.siteInfoGroup.siteInfoThresTemp3 = sMenu_Variable.u16ThresTemp[2];
  privateMibBase.siteInfoGroup.siteInfoThresTemp4 = sMenu_Variable.u16ThresTemp[3];
  privateMibBase.siteInfoGroup.siteInfoMeasuredTemp = sAirCon_Variable.indorTemp;
  privateMibBase.siteInfoGroup.siteInfoMeasuredHumid = 80;
  
  privateMibBase.accessoriesGroup.airCon1Status = sAirCon_Variable.airCon1Status; //1: Run/On; 0: Stop/Off
  privateMibBase.accessoriesGroup.airCon2Status = sAirCon_Variable.airCon2Status; //1: Run/On; 0: Stop/Off
  privateMibBase.accessoriesGroup.fan1Status = sAirCon_Variable.fanStatus; //1: Run/On; 0: Stop/Off  
  privateMibBase.accessoriesGroup.airConSetTemp1 = sMenu_Variable.u16AirConTemp[0];
  privateMibBase.accessoriesGroup.airConSetTemp2 = sMenu_Variable.u16AirConTemp[1];
  privateMibBase.accessoriesGroup.airConSetTemp3 = sMenu_Variable.u16AirConTemp[2];
  privateMibBase.accessoriesGroup.airConSetTemp4 = sMenu_Variable.u16AirConTemp[3];  
  
  // Fire Alarm
  if (DigitalInput[1] == 1) 
  {
    privateMibBase.alarmGroup.alarmFireAlarms = 1;
    sActive_Alarm[2].status = 1;
    sActive_Alarm[3].status = 0;
  } else if (DigitalInput[1] == 2) 
  {
    privateMibBase.alarmGroup.alarmFireAlarms = 2;
    sActive_Alarm[2].status = 0;
    sActive_Alarm[3].status = 1;    
  } else
  {
    privateMibBase.alarmGroup.alarmFireAlarms = 0;
    sActive_Alarm[2].status = 0;
    sActive_Alarm[3].status = 0;
  }
  // Smoke Alarm
  if (DigitalInput[2] == 1) 
  {
    privateMibBase.alarmGroup.alarmSmokeAlarms = 1;
    sActive_Alarm[4].status = 1;
    sActive_Alarm[5].status = 0;
  } else if(DigitalInput[2] == 2) 
  {
    privateMibBase.alarmGroup.alarmSmokeAlarms = 2;
    sActive_Alarm[4].status = 0;
    sActive_Alarm[5].status = 1;    
  } else
  {
    privateMibBase.alarmGroup.alarmSmokeAlarms = 0;
    sActive_Alarm[4].status = 0;
    sActive_Alarm[5].status = 0;
  }
  // Move Alarm
  if (DigitalInput[3] == 1) 
  {
    privateMibBase.alarmGroup.alarmMotionDetectAlarms = 1;
    sActive_Alarm[6].status = 1;
    sActive_Alarm[7].status = 0;
  } else if(DigitalInput[3] == 2) 
  {
    privateMibBase.alarmGroup.alarmMotionDetectAlarms = 2;
    sActive_Alarm[6].status = 0;
    sActive_Alarm[7].status = 1;    
  } else
  {
    privateMibBase.alarmGroup.alarmMotionDetectAlarms = 0;
    sActive_Alarm[6].status = 0;
    sActive_Alarm[7].status = 0;
  }
  // FloodDetectAlarm
  if (DigitalInput[4] == 1) 
  {
    privateMibBase.alarmGroup.alarmFloodDetectAlarms = 1;
    sActive_Alarm[8].status = 1;
    sActive_Alarm[9].status = 0;
  } else if(DigitalInput[4] == 2) 
  {
    privateMibBase.alarmGroup.alarmFloodDetectAlarms = 2;
    sActive_Alarm[8].status = 0;
    sActive_Alarm[9].status = 1;    
  } else
  {
    privateMibBase.alarmGroup.alarmFloodDetectAlarms = 0;
    sActive_Alarm[8].status = 0;
    sActive_Alarm[9].status = 0;
  }
  // DoorOpenAlarm
  if (DigitalInput[5] == 1) 
  {
    privateMibBase.alarmGroup.alarmDoorOpenAlarms = 1;
    sActive_Alarm[10].status = 1;
    sActive_Alarm[11].status = 0;
  } else if(DigitalInput[5] == 2) 
  {
    privateMibBase.alarmGroup.alarmDoorOpenAlarms = 2;
    sActive_Alarm[10].status = 0;
    sActive_Alarm[11].status = 1;    
  } else
  {
    privateMibBase.alarmGroup.alarmDoorOpenAlarms = 0;
    sActive_Alarm[10].status = 0;
    sActive_Alarm[11].status = 0;
  }
  // chaunm-01/04/2018
  // if accessUID != 1 --> valid user --> alarm access = userID
  // if accessUID != -1 --> invalid user --> alarm access = 0
  // use the 8th bit to mark there is a RF card detect
  if(newCardDetect ==1)
  {
    newCardDetect = 0;
    privateMibBase.siteInfoGroup.siteInfoAccessIdLen = 8;
    memcpy(privateMibBase.siteInfoGroup.siteInfoAccessId, (const char*)AccessIdTemp, 8);
    if (sMenu_Control.accessUID == -1)
    {
      //privateMibBase.alarmGroup.alarmAccessAlarms = 1;
      
      privateMibBase.alarmGroup.alarmAccessAlarms &= 0x00; 
      privateMibBase.alarmGroup.alarmAccessAlarms ^= 0x100; 
      IO_OPENDOOR_MCU_ON();    
    } else if (sMenu_Control.accessUID != 0)
    {
      //privateMibBase.alarmGroup.alarmAccessAlarms = 0;
      privateMibBase.alarmGroup.alarmAccessAlarms &= 0x00; // clear last ID
      privateMibBase.alarmGroup.alarmAccessAlarms = sMenu_Control.accessUID; // add current ID
      privateMibBase.alarmGroup.alarmAccessAlarms ^= 0x100; // change mark bit
      IO_OPENDOOR_MCU_OFF();
    } 
  }
  for (j=0;j<5;j++)
  {
    privateMibBase.configGroup.configAccessIdTable[j].configAccessIdIndex = j+1;
    privateMibBase.configGroup.configAccessIdTable[j].configAccessIdCardLen = 8;
    memset(&privateMibBase.configGroup.configAccessIdTable[j].configAccessIdCard,0,16);
    for(i=0;i<8;i++)    
      privateMibBase.configGroup.configAccessIdTable[j].configAccessIdCard[i] = sMenu_Variable.u8UserID[j][i];    
  }    
  // chaunm - already update 08/04
//  for(i=0;i<8;i++)
//  {
//    privateMibBase.siteInfoGroup.siteInfoAccessId[i] = AccessIdTemp[i];
//    privateMibBase.siteInfoGroup.siteInfoAccessIdLen = 8;
//  }
  
  Alarm_Control();
  Relay_Output();
}

void Alarm_Control(void)
{
  if((privateMibBase.alarmGroup.alarmFireAlarms != 0)  ||
     (privateMibBase.alarmGroup.alarmSmokeAlarms != 0)  ||
       (privateMibBase.alarmGroup.alarmMotionDetectAlarms != 0)  ||
         (privateMibBase.alarmGroup.alarmFloodDetectAlarms != 0)  ||
           (privateMibBase.alarmGroup.alarmDoorOpenAlarms != 0)  ||
             (privateMibBase.alarmGroup.alarmGenFailureAlarms != 0)  ||
               (privateMibBase.alarmGroup.alarmDcThresAlarms != 0)  ||
                 (privateMibBase.alarmGroup.alarmMachineStopAlarms != 0)  ||
                   (privateMibBase.alarmGroup.alarmAccessAlarms != 0)  ||
                     (privateMibBase.alarmGroup.alarmAcThresAlarms != 0)  )
  {
    privateMibBase.accessoriesGroup.ledControlStatus = 1;
    privateMibBase.accessoriesGroup.speakerControlStatus = 1;
  } else
  {
    privateMibBase.accessoriesGroup.ledControlStatus = 0;
    privateMibBase.accessoriesGroup.speakerControlStatus = 0;
  }      
}

void Relay_Output(void)
{
  if (privateMibBase.accessoriesGroup.ledControlStatus == 1)
  {
    IO_ALARM_LIGHT_ON(); 
  } else if (privateMibBase.accessoriesGroup.ledControlStatus == 0)
  {
    IO_ALARM_LIGHT_OFF();   
  }
  
  if (privateMibBase.accessoriesGroup.speakerControlStatus == 1)
  {
    IO_ALARM_SPEAKER_ON(); 
  } else if (privateMibBase.accessoriesGroup.speakerControlStatus == 0)
  {   
    IO_ALARM_SPEAKER_OFF();
  }  
}


