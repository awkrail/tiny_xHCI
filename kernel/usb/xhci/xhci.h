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
#include "port.h"

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

/**
 * Initialize xHC host controller
**/
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
enum Error EnableInterruptForPrimaryInterrupter(struct InterrupterRegisterSet 
                                                *primary_interrupter);

// Initialize Register Set Array
enum Error InitializeInterruptRegisterSetArray(struct Controller *xhc,
                                               volatile struct InterrupterRegisterSetArrayWrapper
                                               *primary_interrupter);

/**
 * Initiealize Port and detect devices
**/
struct Port xHCIPortAt(struct Controller *xhc,
                       uint8_t port_num);
enum Error xHCIConfigurePort(struct Controller *xhc,
                             struct Port *port);
enum Error xHCIResetPort(struct Controller *xhc,
                         struct Port *port);
enum Error xHCIEnableSlot(struct Controller *xhc,
                          struct Port *port);

/**
 * Process Events
**/
enum Error OnPortStatusChangeEvent(struct Controller *xhc,
                                   union PortStatusChangeEventTRB *trb);

enum Error xHCIProcessEvent(struct Controller *xhc);


// Register-related functions
uint8_t ReadCAPLENGTH(volatile struct CapabilityRegisters *cap);
uint16_t ReadHCIVERSION(volatile struct CapabilityRegisters *cap);

// Event ring functions
void WriteDequeuePointer(union TRB *p, struct InterrupterRegisterSet *interrupter);

// Debug
void PrintAllRegisters(struct Controller *xhc,
                       struct Console *console);
