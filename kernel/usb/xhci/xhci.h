#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../../error.h"
#include "../../logger.h"
#include "devmgr.h"
#include "registers.h"

#define kDeviceSize 8

struct Controller
{
  uintptr_t mmio_base;
  volatile struct CapabilityRegisters *cap;
  volatile struct OperationalRegisters *op;
};

// Controller-related functions
void InitializeController(struct DeviceManager *dev_mgr,
                          struct Controller *xhc,
                          uintptr_t mmio_base,
                          struct Console *console);
void PrintAllRegisters(struct Controller *xhc,
                       struct Console *console);
void SetCapAndOpRegisters(struct Controller *xhc,
                          struct Console *console);
void ResetController(struct Controller *xhc,
                     struct Console *console);
void SetMaxSlotEnabled(struct Controller* xhc,
                       struct Console *console);

// Register-related functions
uint8_t ReadCAPLENGTH(volatile struct CapabilityRegisters *cap);
uint16_t ReadHCIVERSION(volatile struct CapabilityRegisters *cap);
