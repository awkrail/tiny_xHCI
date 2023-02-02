#include "xhci.h"

void InitializeController(struct Controller *xhc,
                          uintptr_t mmio_base)
{
  xhc->mmio_base = mmio_base;
  xhc->cap = (struct CapabilityRegisters*)mmio_base;
  xhc->op = (struct OperationalRegisters*)(mmio_base + ReadCAPLENGTH(xhc->cap));
  //xhc->max_ports = (uint8_t)cap.HCSPARAMS1; // Read.bits().max_ports..
}

uint8_t ReadCAPLENGTH(const struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION & 0xffu;
}

uint16_t ReadHCIVERSION(const struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION >> 16;
}

