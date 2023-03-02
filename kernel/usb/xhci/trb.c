#include "trb.h"

union TransferEventTRB* CastTRBtoTransferEventTRB(union TRB* trb)
{
  if(kTransferEventTRBType == trb->bits.trb_type) {
    return (union TransferEventTRB*)trb;
  }
  return NULL;
}

union PortStatusChangeEventTRB* CastTRBToPortStatusChangeEventTRB(union TRB* trb)
{
  if(kPortStatusChangeEventTRBType == trb->bits.trb_type) {
    return (union PortStatusChangeEventTRB*)trb;
  }
  return NULL;
}

union CommandCompletionEventTRB* CastTRBToCommandCompletionEventTRB(union TRB* trb)
{
  if(kCommandCompletionEventTRBType == trb->bits.trb_type) {
    return (union CommandCompletionEventTRB*)trb;
  }
  return NULL;
}

union EnableSlotCommandTRB InitializeEnableSlotCommandTRB()
{
  union EnableSlotCommandTRB cmd;
  for(int i=0; i<4; ++i) {
    cmd.data[i] = 0x00000000;
  }
  cmd.bits.trb_type = kEnableSlotCommandTRBType;
  return cmd;
}

union LinkTRB InitializeLinkTRB(union TRB* ring_segment_pointer)
{
  union LinkTRB link;
  for(int i=0; i<4; ++i) {
    link.data[i] = 0x00000000;
  }
  link.bits.ring_segment_pointer = (uint64_t)ring_segment_pointer >> 4;
  return link;
}
