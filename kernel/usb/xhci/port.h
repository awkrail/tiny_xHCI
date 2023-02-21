#pragma once

#include <stdint.h>
#include "stdbool.h"
#include "../../logger.h"
#include "../../error.h"
#include "registers.h"

struct Port
{
  uint8_t port_num;
  struct PortRegisterSet port_reg_set;
};

bool IsPortConnected(struct Port *port);
bool IsPortEnabled(struct Port *port);
bool IsPortConnectStatusChanged(struct Port *port);
bool IsPortResetChanged(struct Port *port);

int PortSpeed(struct Port *port);
enum Error ResetPort(struct Port *port);
