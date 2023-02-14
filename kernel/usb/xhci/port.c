#include "port.h"

bool IsPortConnected(struct Port *port)
{
  return port->port_reg_set->PORTSC.bits.current_connect_status;
}

bool IsPortEnabled(struct Port *port)
{
  return port->port_reg_set->PORTSC.bits.port_enabled_disabled;
}

bool IsPortConnectStatusChanged(struct Port *port)
{
  return port->port_reg_set->PORTSC.bits.connect_status_change;
}

bool IsPortResetChanged(struct Port *port)
{
  return port->port_reg_set->PORTSC.bits.port_reset_change;
}

int PortSpeed(struct Port *port)
{
  return port->port_reg_set->PORTSC.bits.port_speed;
}

enum Error ResetPort(struct Port *port)
{
  port->port_reg_set->PORTSC.data &= 0x0e00c3e0u;
  port->port_reg_set->PORTSC.data |= 0x00020010u;
  while(port->port_reg_set->PORTSC.bits.port_reset);
  return kSuccess;
}
