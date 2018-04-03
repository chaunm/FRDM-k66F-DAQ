#ifndef __SNMP_CLIENT_H__
#define __SNMP_CLIENT_H__

void SnmpInitMib();
error_t SnmpInitClient(NetInterface* interface);
void SnmpSendTrapTask(void *param);
#endif // __SNMP_CLIENT_H__