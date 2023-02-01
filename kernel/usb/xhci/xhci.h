#pragma once

#include "error.h"

#define kDeviceSize 8

struct Controller
{
  const uintptr_t mmio_base;
  CapabilityRegisters *cap;
  OperationalRegisters *op;
  uint8_t max_ports;
  struct DeviceManager devmgr;
  struct Ring cr;
  struct EventRing er;
};

void InitializeController(struct Controller *xhc,
                          uintptr_t mmio_base);

enum Error InitializeXhci(struct Controller *xhc);
