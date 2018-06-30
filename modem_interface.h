#ifndef __MODEM_INTERFACE_H__
#define __MODEM_INTERFACE_H__
#include "core/net.h"

typedef enum 
{
  MODEM_INTERFACE_STATE_DISCONNECTED = 0,
  MODEM_INTERFACE_STATE_CONNECTED
} modem_interface_state_t;

typedef struct {
  uint8_t setState;
  uint8_t currentState;
} modem_interface_manage_t;

NetInterface* ModemInterfaceInit();
void ModemInterfaceSetState(modem_interface_state_t state);
uint8_t ModemInterfaceGetState();
void ModemInterfaceTask(void* param);

#endif