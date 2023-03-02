#include "xhci.h"

enum PortConfigPhase {
  kPortConfigPhaseNotConnected,
  kPortConfigPhaseWaitingAddressed,
  kPortConfigPhaseResettingPort,
  kPortConfigPhaseEnablingSlot,
  kPortConfigPhaseAddressingDevice,
  kPortConfigPhaseInitializingDevice,
  kPortConfigPhaseConfiguringEndpoints,
  kPortConfigPhaseConfigured,
};

/** executing addressing ports
 *  0 -> no ports
*/
volatile enum PortConfigPhase port_config_phase[256] = {0};
volatile uint8_t addressing_port = 0;


enum Error InitializeController(struct DeviceManager *dev_mgr,
                                struct Controller *xhc,
                                uintptr_t mmio_base)
{
  // Initialize device manager (e.g., DeviceContext)
  enum Error err = InitializeDevMgr(dev_mgr, kDeviceSize);
  xhc->dev_mgr = dev_mgr;

  // set registers
  xhc->mmio_base = mmio_base;
  err = SetCapAndOpRegisters(xhc);

  // reset controller
  err = ResetController(xhc);

  // set Max Slots Enabled
  err = SetMaxSlotEnabled(xhc);

  // Set DCBAAP Register
  err = SetDCBAAPRegister(xhc);

  // Command Ring configuration
  err = InitializeCommandRing(xhc, 32);
  err = RegisterCommandRing(xhc);

  // Event Ring configuration
  //volatile struct InterrupterRegisterSetArrayWrapper primary_interrupter;
  struct InterrupterRegisterSetArrayWrapper primary_interrupter 
    = InitializeInterruptRegisterSetArray(xhc);

  err = InitializeEventRing(xhc, primary_interrupter.array, 32);

  // Enable interrupt for the primary interrupter
  err = EnableInterruptForPrimaryInterrupter(primary_interrupter.array);

  // Run controller
  err = StartController(xhc);

  return err;
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

enum Error SetCapAndOpRegisters(struct Controller *xhc)
{
  uintptr_t mmio_base = xhc->mmio_base;
  xhc->cap = (struct CapabilityRegisters*)mmio_base;
  xhc->op = (struct OperationalRegisters*)(mmio_base + ReadCAPLENGTH(xhc->cap));
  xhc->max_ports = xhc->cap->HCSPARAMS1.bits.max_ports;
  return kSuccess;
}

enum Error ResetController(struct Controller *xhc)
{
  xhc->op->USBCMD.bits.interrupter_enable = false;
  xhc->op->USBCMD.bits.host_system_error_enable = false;
  xhc->op->USBCMD.bits.enable_wrap_event = false;
  if(!xhc->op->USBSTS.bits.host_controller_halted)
    xhc->op->USBCMD.bits.run_stop = false;
  xhc->op->USBCMD.bits.host_controller_reset = true;
  
  while(xhc->op->USBCMD.bits.host_controller_reset);
  while(xhc->op->USBSTS.bits.controller_not_ready);
  return kSuccess;
}

enum Error SetMaxSlotEnabled(struct Controller *xhc)
{
  xhc->op->CONFIG.bits.max_device_slots_enabled = kDeviceSize;
  return kSuccess;
}

enum Error SetDCBAAPRegister(struct Controller *xhc)
{
  uint64_t value = (uint64_t)xhc->dev_mgr->device_context_ptrs;
  xhc->op->DCBAAP.bits.device_context_base_address_array_pointer = value >> 6;
  return kSuccess;
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
  WriteDequeuePointer(&xhc->er.buf[0], interrupter);
  interrupter->ERSTBA.bits.event_ring_segment_table_base_address = ((uint64_t)xhc->er.erst) >> 6;
  return kSuccess;
}

void WriteDequeuePointer(union TRB *p, struct InterrupterRegisterSet *interrupter)
{
  uint64_t val = (uint64_t)p;
  interrupter->ERDP.bits.event_ring_dequeue_pointer = val >> 4;
}


enum Error EnableInterruptForPrimaryInterrupter(struct InterrupterRegisterSet *primary_interrupter)
{
  primary_interrupter->IMAN.bits.interrupt_pending = true;
  primary_interrupter->IMAN.bits.interrupt_enable = true;
  return kSuccess;
}

enum Error StartController(struct Controller *xhc)
{
  xhc->op->USBCMD.bits.interrupter_enable = true;
  xhc->op->USBCMD.bits.run_stop = true;
  while(xhc->op->USBSTS.bits.host_controller_halted);
  return kSuccess;
}

struct Port xHCIPortAt(struct Controller *xhc,
                       uint8_t port_num)
{
  struct PortRegisterSetArrayWrapper port_reg = InitializePortRegisterSetArray(xhc);
  
  struct Port port;
  port.port_num = port_num;
  port.port_reg_set = &port_reg.array[port_num-1];
  return port;
}

enum Error xHCIConfigurePort(struct Controller *xhc, struct Port *port)
{
  if(port_config_phase[port->port_num] == kPortConfigPhaseNotConnected) {
    return xHCIResetPort(xhc, port);
  }
  return kSuccess;
}

enum Error xHCIResetPort(struct Controller *xhc, struct Port *port)
{
  bool is_connected = IsPortConnected(port);

  if(!is_connected) {
    return kSuccess;
  }

  if(addressing_port != 0) {
    port_config_phase[port->port_num] = kPortConfigPhaseWaitingAddressed;
  } else {
    enum PortConfigPhase port_phase = port_config_phase[port->port_num];

    if(port_phase != kPortConfigPhaseNotConnected &&
       port_phase != kPortConfigPhaseWaitingAddressed)
      return kInvalidPhase;

    addressing_port = port->port_num;
    port_config_phase[port->port_num] = kPortConfigPhaseResettingPort;
    ResetPort(port);
  }
  return kSuccess;  
}

enum Error xHCIEnableSlot(struct Controller *xhc, struct Port *port)
{
  bool is_connected = IsPortConnected(port);
  bool reset_completed = IsPortResetChanged(port);
  if(is_connected && reset_completed) {
    ClearPortResetChange(port);
    port_config_phase[port->port_num] = kPortConfigPhaseEnablingSlot;
    union EnableSlotCommandTRB cmd = InitializeEnableSlotCommandTRB();
    PushCommandRing(&xhc->cr, cmd.data);
    union Doorbell_Bitmap* doorbell = DoorbellRegisterAt(xhc, 0);
    doorbell->bits.db_target = 0;
    doorbell->bits.db_stream_id = 0;
  }
  return kSuccess;
}

union Doorbell_Bitmap* DoorbellRegisterAt(struct Controller *xhc, size_t index)
{
  struct DoorbellRegisterSetArrayWrapper doorbell 
    = InitializeDoorbellRegisterSetArray(xhc);
  return &doorbell.array[index];
}


enum Error OnPortStatusChangeEvent(struct Controller *xhc, union PortStatusChangeEventTRB *trb)
{
  uint32_t port_id = trb->bits.port_id;
  struct Port port = xHCIPortAt(xhc, port_id);

  switch(port_config_phase[port_id]) {
    case kPortConfigPhaseNotConnected:
      return xHCIResetPort(xhc, &port);
    case kPortConfigPhaseResettingPort:
      return xHCIEnableSlot(xhc, &port);
    default:
      return kInvalidPhase;
  }
}

enum Error xHCIProcessEvent(struct Controller *xhc)
{
  if(!HasEventRingFront(&xhc->er))
    return kSuccess;

  enum Error err = kNotImplemented;
  union TRB *event_trb = EventRingFront(&xhc->er);
  void* trb;

  union TransferEventTRB *transfer_event_trb = NULL;
  union PortStatusChangeEventTRB *port_status_change_event_trb = NULL;
  union CommandCompletionEventTRB *command_completion_event_trb = NULL;

  if((trb = CastTRBtoTransferEventTRB(event_trb)) != NULL) {
    transfer_event_trb = (union TransferEventTRB*)trb;
    //err = OnTransferEvent(xhc, *trb);
  } else if ((trb = CastTRBToPortStatusChangeEventTRB(event_trb)) != NULL) {
    port_status_change_event_trb = (union PortStatusChangeEventTRB*)trb;
    err = OnPortStatusChangeEvent(xhc, port_status_change_event_trb);
  } else if ((trb = CastTRBToCommandCompletionEventTRB(event_trb)) != NULL) {
    command_completion_event_trb = (union CommandCompletionEventTRB*)trb;
    //err = OnCommandCompletionEvent(xhc, *trb);
  }
  //Pop(&xhc->er);

  return kSuccess;
}

struct InterrupterRegisterSetArrayWrapper 
InitializeInterruptRegisterSetArray(struct Controller *xhc)
{
  struct InterrupterRegisterSetArrayWrapper primary_interrupter;
  primary_interrupter.array = (struct InterrupterRegisterSet*)
    (xhc->mmio_base + (xhc->cap->RTSOFF.bits.runtime_register_space_offset << 5) + 0x20);
  primary_interrupter.size = 1024;
  return primary_interrupter;
}

struct PortRegisterSetArrayWrapper
InitializePortRegisterSetArray(struct Controller *xhc)
{
  struct PortRegisterSetArrayWrapper port_reg;
  port_reg.array = (struct PortRegisterSet*)((uintptr_t)xhc->op + 0x400u);
  port_reg.size = xhc->max_ports;
  return port_reg;
}

struct DoorbellRegisterSetArrayWrapper
InitializeDoorbellRegisterSetArray(struct Controller *xhc)
{
  struct DoorbellRegisterSetArrayWrapper doorbell;
  doorbell.array = (union Doorbell_Bitmap*)(xhc->mmio_base + (xhc->cap->DBOFF.bits.doorbell_array_offset << 2));
  doorbell.size = 256;
  return doorbell;
}

uint8_t ReadCAPLENGTH(volatile struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION & 0xffu;
}

uint16_t ReadHCIVERSION(volatile struct CapabilityRegisters *cap)
{
  return cap->CAPLENGTH_HCIVERSION >> 16;
}

