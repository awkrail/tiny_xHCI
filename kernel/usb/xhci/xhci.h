#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "stdbool.h"

#include "../../error.h"
#include "../../logger.h"
#include "devmgr.h"
#include "registers.h"
#include "ring.h"
#include "trb.h"

#define kDeviceSize 8

struct Controller
{
  uintptr_t mmio_base;
  volatile struct CapabilityRegisters *cap;
  volatile struct OperationalRegisters *op;
  volatile struct DeviceManager *dev_mgr;
  uint8_t max_ports;
  struct Ring cr;
  struct EventRing er;
};

// Controller-related functions
enum Error InitializeController(struct DeviceManager *dev_mgr,
                          struct Controller *xhc,
                          uintptr_t mmio_base);
enum Error SetCapAndOpRegisters(struct Controller *xhc);
enum Error ResetController(struct Controller *xhc);
enum Error SetMaxSlotEnabled(struct Controller* xhc);
enum Error SetDCBAAPRegister(struct Controller *xhc);
enum Error StartController(struct Controller *xhc);

// Initialize Command ring
enum Error InitializeCommandRing(struct Controller *xhc, size_t buf_size);
enum Error RegisterCommandRing(struct Controller *xhc);

// Initialize Event Ring
enum Error InitializeEventRing(struct Controller *xhc,
                               struct InterrupterRegisterSet *interrupter,
                               size_t buf_size);

// Initialize Register Set Array
enum Error InitializeInterruptRegisterSetArray(struct Controller *xhc,
                                               volatile struct InterrupterRegisterSetArrayWrapper
                                               *primary_interrupter);
enum Error EnableInterruptForPrimaryInterrupter(struct InterrupterRegisterSet 
                                                *primary_interrupter);

// Register-related functions
uint8_t ReadCAPLENGTH(volatile struct CapabilityRegisters *cap);
uint16_t ReadHCIVERSION(volatile struct CapabilityRegisters *cap);

// Debug
void PrintAllRegisters(struct Controller *xhc,
                       struct Console *console);
