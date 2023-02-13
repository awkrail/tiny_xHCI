#pragma once

#include <stddef.h>
#include <stdint.h>

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
  //struct EventRing er;
};

// Controller-related functions
void InitializeController(struct DeviceManager *dev_mgr,
                          struct Controller *xhc,
                          uintptr_t mmio_base,
                          struct Console *console);
void PrintAllRegisters(struct Controller *xhc,
                       struct Console *console);
void SetCapAndOpRegisters(struct Controller *xhc);
void ResetController(struct Controller *xhc);
void SetMaxSlotEnabled(struct Controller* xhc);
void SetDCBAAPRegister(struct Controller *xhc);

// Initialize Command ring
enum Error InitializeCommandRing(struct Controller *xhc, size_t buf_size);
enum Error RegisterCommandRing(struct Controller *xhc);

// Initialize Event Ring


// Register-related functions
uint8_t ReadCAPLENGTH(volatile struct CapabilityRegisters *cap);
uint16_t ReadHCIVERSION(volatile struct CapabilityRegisters *cap);
