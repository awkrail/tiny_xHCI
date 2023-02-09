#include "string.h"
#include "stdbool.h"
#include "xhci.h"
#include "devmgr.h"

void InitializeController(struct DeviceManager *dev_mgr,
                          struct Controller *xhc,
                          uintptr_t mmio_base,
                          struct Console *console)
{
  // Initialize device manager (e.g., DeviceContext)
  // TODO: Implement them!
  enum Error err = InitializeDevMgr(kDeviceSize);

  // set registers
  xhc->mmio_base = mmio_base;
  SetCapAndOpRegisters(xhc, console);

  // reset controller
  ResetController(xhc, console);

  // set Max Slots Enabled
  SetMaxSlotEnabled(xhc, console);

  PrintAllRegisters(xhc, console);
}

// for debug
void PrintAllRegisters(struct Controller *xhc,
                       struct Console *console)
{
  volatile struct CapabilityRegisters *cap = xhc->cap;
  volatile struct OperationalRegisters *op = xhc->op;

  Log(kDebug,
      console,
      "len(CapabilityRegisters)=%u\n",
      sizeof(struct CapabilityRegisters));

  Log(kDebug,
      console,
      "len(OperationalRegisters)=%u\n",
      sizeof(struct OperationalRegisters));

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
      "CONFIG=%08x\n",
      op->USBCMD, op->USBSTS,
      op->DCBAAP, op->CONFIG.data);
}

void SetCapAndOpRegisters(struct Controller *xhc,
                          struct Console *console)
{
  uintptr_t mmio_base = xhc->mmio_base;
  xhc->cap = (struct CapabilityRegisters*)mmio_base;
  xhc->op = (struct OperationalRegisters*)(mmio_base + ReadCAPLENGTH(xhc->cap));
}

void ResetController(struct Controller *xhc,
                     struct Console *console)
{
  xhc->op->USBCMD.bits.interrupter_enable = false;
  xhc->op->USBCMD.bits.host_system_error_enable = false;
  xhc->op->USBCMD.bits.enable_wrap_event = false;
  if(!xhc->op->USBSTS.bits.host_controller_halted)
    xhc->op->USBCMD.bits.run_stop = false;
  xhc->op->USBCMD.bits.host_controller_reset = true;
  
  while(xhc->op->USBCMD.bits.host_controller_reset);
  while(xhc->op->USBSTS.bits.controller_not_ready);
}

void SetMaxSlotEnabled(struct Controller *xhc,
                       struct Console *console)
{
  xhc->op->CONFIG.bits.max_device_slots_enabled = kDeviceSize;
}

void AllocateMemory(struct Controller *xhc,
                    struct Console *console)
{
  const uint16_t max_scratchpad_buffers =
    xhc->cap->HCSPARAMS2.bits.max_scratchpad_buffers_low
    | (xhc->cap->HCSPARAMS2.bits.max_scratchpad_buffers_high << 5);
  if(max_scratchpad_buffers > 0) {
    // allocate memories...
  }
}


uint8_t ReadCAPLENGTH(volatile struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION & 0xffu;
}

uint16_t ReadHCIVERSION(volatile struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION >> 16;
}

