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
