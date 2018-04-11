/**
 * @file private_mib_impl.h
 * @brief Private MIB module implementation
 *
 * @section License
 *
 * ^^(^____^)^^
 **/

#ifndef _PRIVATE_MIB_IMPL_H
#define _PRIVATE_MIB_IMPL_H

//Dependencies
#include "mibs/mib_common.h"
#include "snmp/snmp_agent.h"

//Private MIB related functions
error_t privateMibInit(void);
void privateMibLock(void);
void privateMibUnlock(void);
void UpdateInfo (void);
void Alarm_Control(void);
void Relay_Output(void);
uint8_t IsAnyAlarm();


error_t privateMibGetCurrentTime(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibSetStringEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen);

error_t privateMibSetLedEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen);

error_t privateMibGetLedEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibGetNextLedEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

//=============================================== Main Function ============================================//
error_t privateMibSetAcPhaseEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen);

error_t privateMibGetAcPhaseEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibGetNextAcPhaseEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

error_t privateMibGetConfigAccEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibGetNextConfigAccEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

error_t privateMibSetConfigAccessIdEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen);

error_t privateMibGetConfigAccessIdEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibGetNextConfigAccessIdEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

error_t privateMibSetAccessoriesGroup(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen);

error_t privateMibGetAccessoriesGroup(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibGetConfigInfoGroup(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibSetSiteInfoGroup(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen);

error_t privateMibGetSiteInfoGroup(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibGetAlarmGroup(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t privateMibSetBatteryGroup(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen);

error_t privateMibGetBatteryGroup(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

#endif
