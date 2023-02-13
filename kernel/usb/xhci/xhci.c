#include "xhci.h"

void InitializeController(struct DeviceManager *dev_mgr,
                          struct Controller *xhc,
                          uintptr_t mmio_base,
                          struct Console *console)
{
  // Initialize device manager (e.g., DeviceContext)
  enum Error err = InitializeDevMgr(dev_mgr, kDeviceSize);
  xhc->dev_mgr = dev_mgr;

  // set registers
  xhc->mmio_base = mmio_base;
  SetCapAndOpRegisters(xhc);

  // reset controller
  ResetController(xhc);

  // set Max Slots Enabled
  SetMaxSlotEnabled(xhc);

  // Set DCBAAP Register
  SetDCBAAPRegister(xhc);

  // Command Ring configuration
  err = InitializeCommandRing(xhc, 32);
  err = RegisterCommandRing(xhc);

  // Event Ring configuration
  volatile struct InterrupterRegisterSetArrayWrapper primary_interrupter;
  InitializeInterruptRegisterSetArray(xhc, &primary_interrupter);

  err = InitializeEventRing(xhc, primary_interrupter.array, 32);

  // Enable interrupt for the primary interrupter
  EnableInterruptForPrimaryInterrupter(primary_interrupter.array);

  // Run controller
  StartController(xhc);
  //return err;
  
  Log(kDebug, console, "err: %s\n", GetErrName(err));
}

// for debug
void PrintAllRegisters(struct Controller *xhc,
                       struct Console *console)
{
  volatile struct CapabilityRegisters *cap = xhc->cap;
  volatile struct OperationalRegisters *op = xhc->op;

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
      "CRCR=%08x\n"
      "DCBAAP=%08x\n"
      "CONFIG=%08x\n",
      op->USBCMD, op->USBSTS,
      op->CRCR, op->DCBAAP, op->CONFIG);
}

void SetCapAndOpRegisters(struct Controller *xhc)
{
  uintptr_t mmio_base = xhc->mmio_base;
  xhc->cap = (struct CapabilityRegisters*)mmio_base;
  xhc->op = (struct OperationalRegisters*)(mmio_base + ReadCAPLENGTH(xhc->cap));
  xhc->max_ports = xhc->cap->HCSPARAMS1.bits.max_ports;
}

void ResetController(struct Controller *xhc)
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

void SetMaxSlotEnabled(struct Controller *xhc)
{
  xhc->op->CONFIG.bits.max_device_slots_enabled = kDeviceSize;
}

void SetDCBAAPRegister(struct Controller *xhc)
{
  uint64_t value = (uint64_t)xhc->dev_mgr->device_context_ptrs;
  xhc->op->DCBAAP.bits.device_context_base_address_array_pointer = value >> 6;
}

void InitializeInterruptRegisterSetArray(struct Controller *xhc,
                                         volatile struct InterrupterRegisterSetArrayWrapper 
                                          *primary_interrupter)
{
  primary_interrupter->array = (struct InterrupterRegisterSet*)
    (xhc->mmio_base + (xhc->cap->RTSOFF.bits.runtime_register_space_offset << 5) + 0x20);
  primary_interrupter->size = 1024;
}

enum Error InitializeCommandRing(struct Controller *xhc, size_t buf_size)
{
  if(xhc->cr.buf != NULL)
    FreeMem(xhc->cr.buf);

  xhc->cr.cycle_bit = true;
  xhc->cr.write_index = 0;
  xhc->cr.buf_size = buf_size;

  xhc->cr.buf = AllocTRBArray(buf_size, 64, 64 * 1024);
  if(xhc->cr.buf == NULL)
    return kNoEnoughMemory;

  memset(xhc->cr.buf, 0, buf_size * sizeof(union TRB));
  return kSuccess;
}

enum Error RegisterCommandRing(struct Controller *xhc)
{

  xhc->op->CRCR.bits.ring_cycle_state = true;
  xhc->op->CRCR.bits.command_stop = false;
  xhc->op->CRCR.bits.command_abort = false;
  xhc->op->CRCR.bits.command_ring_pointer = ((uint64_t)xhc->cr.buf) >> 6;
  return kSuccess;
}

enum Error InitializeEventRing(struct Controller *xhc,
                               struct InterrupterRegisterSet *interrupter,
                               size_t buf_size)
{
  if(xhc->er.buf != NULL)
    FreeMem(xhc->er.buf);

  xhc->er.cycle_bit = true;
  xhc->er.buf_size = buf_size;
  xhc->er.interrupter = interrupter;

  xhc->er.buf = AllocTRBArray(buf_size, 64, 64 * 1024);
  if(xhc->er.buf == NULL)
    return kNoEnoughMemory;
  memset(xhc->er.buf, 0, buf_size * sizeof(union TRB));

  xhc->er.erst = AllocEventRingSegmentTableEntryArray(1, 64, 64 * 1024);
  if(xhc->er.erst == NULL) {
    FreeMem(xhc->er.buf);
    return kNoEnoughMemory;
  }
  memset(xhc->er.erst, 0, 1 * sizeof(union EventRingSegmentTableEntry));

  xhc->er.erst[0].bits.ring_segment_base_address = (uint64_t)xhc->er.buf;
  xhc->er.erst[0].bits.ring_segment_size = buf_size;

  interrupter->ERSTSZ.bits.event_ring_segment_table_size = 1;
  interrupter->ERSTBA.bits.event_ring_segment_table_base_address = ((uint64_t)xhc->er.erst) >> 6;
  return kSuccess;
}

void EnableInterruptForPrimaryInterrupter(struct InterrupterRegisterSet *primary_interrupter)
{
  primary_interrupter->IMAN.bits.interrupt_pending = true;
  primary_interrupter->IMAN.bits.interrupt_enable = true;
}

void StartController(struct Controller *xhc)
{
  xhc->op->USBCMD.bits.interrupter_enable = true;
}

uint8_t ReadCAPLENGTH(volatile struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION & 0xffu;
}

uint16_t ReadHCIVERSION(volatile struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION >> 16;
}

