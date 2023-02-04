#include "stdbool.h"
#include "xhci.h"

void InitializeController(struct Controller *xhc,
                          uintptr_t mmio_base,
                          struct Console *console)
{
  // set registers
  xhc->mmio_base = mmio_base;
  SetCapAndOpRegisters(xhc);

  // reset controller
  // RequestHCOwnership();
  //ResetController(xhc);

  // set Max Slots Enabled
  xhc->op->CONFIG.bits.max_device_slots_enabled = 100;
  Log(kDebug, console, "hogehogehoge");
  SetMaxSlotEnabled(xhc, console);
}

void SetCapAndOpRegisters(struct Controller *xhc)
{
  uintptr_t mmio_base = xhc->mmio_base;
  xhc->cap = (struct CapabilityRegisters*)mmio_base;
  xhc->op = (struct OperationalRegisters*)(mmio_base + ReadCAPLENGTH(xhc->cap));
}

void ResetController(struct Controller *xhc)
{
  // maybe this function doesn't work
  union USBCMD_Bitmap usbcmd = xhc->op->USBCMD;
  usbcmd.bits.interrupter_enable = false;
  usbcmd.bits.host_system_error_enable = false;
  usbcmd.bits.enable_wrap_event = false;
  if(!xhc->op->USBSTS.bits.host_controller_halted)
    usbcmd.bits.run_stop = false;

  xhc->op->USBCMD = usbcmd;
  usbcmd.bits.host_controller_reset = true;
  xhc->op->USBCMD = usbcmd;
  while(xhc->op->USBCMD.bits.host_controller_reset);
  while(xhc->op->USBSTS.bits.controller_not_ready);
}

void SetMaxSlotEnabled(struct Controller *xhc,
                       struct Console *console)
{
  //xhc->op->CONFIG.bits.max_device_slots_enabled = 100;
  Log(kDebug, console, "config (max_device_slots): %d\n", xhc->op->CONFIG.bits.max_device_slots_enabled);
  //Log(kDebug, console, "config (u3_entry_enable): %lx\n", xhc->op->CONFIG.bits.u3_entry_enable);
  //Log(kDebug, console, "config (config_info): %lx\n", xhc->op->CONFIG.bits.configuration_information_enable);
  //Log(kDebug, console, "config (reserved): %lx\n", xhc->op->CONFIG.bits.reserved);
}

uint8_t ReadCAPLENGTH(const struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION & 0xffu;
}

uint16_t ReadHCIVERSION(const struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION >> 16;
}

