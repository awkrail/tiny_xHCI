#include "string.h"
#include "stdbool.h"
#include "xhci.h"

void InitializeController(struct Controller *xhc,
                          uintptr_t mmio_base,
                          struct Console *console)
{
  // set registers
  xhc->mmio_base = mmio_base;
  SetCapAndOpRegisters(xhc);

  // for debug
  PrintAllRegisters(xhc, console);

  // reset controller
  // RequestHCOwnership();
  //ResetController(xhc);

  // set Max Slots Enabled
  SetMaxSlotEnabled(xhc, console);
  PrintAllRegisters(xhc, console);
}

// for debug
void PrintAllRegisters(struct Controller *xhc,
                       struct Console *console)
{
  Log(kDebug,
      console,
      "CAPLENGTH=%02x\n"
      "HCIVERSION=%04x\n"
      "DBOFF=%08x\n"
      "RTSOFF=%08x\n"
      "HCSPARAMS1=%08x\n"
      "HCCPARAMS1=%08x\n",
      ReadCAPLENGTH(cap), ReadHCIVERSION(cap),
      cap->DBOFF, cap->RTSOFF, cap->HCSPARAMS1,
      cap->HCCPARAMS1);

  Log(kDebug,
      console,
      "USBCMD=%08x\n"
      "USBSTS=%08x\n"
      "DCBAAP=%08x\n"
      "CONFIG=%08x\n"
      op->USBCMD, op->USBSTS,
      op->DCBAAP, op->CONFIG);
}

void SetCapAndOpRegisters(struct Controller *xhc)
{
  uintptr_t mmio_base = xhc->mmio_base;
  memcpy(xhc->cap, (void*)mmio_base, 
         sizeof(struct CapabilityRegisters));
  memcpy(xhc->op,(void*)(mmio_base+ReadCAPLENGTH(xhc->cap)), 
         sizeof(struct OperationalRegisters));
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
  xhc->op->CONFIG.bits.max_device_slots_enabled = kDeviceSize;
}

uint8_t ReadCAPLENGTH(const struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION & 0xffu;
}

uint16_t ReadHCIVERSION(const struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION >> 16;
}

