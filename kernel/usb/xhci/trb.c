#include "trb.h"

union TransferEventTRB* CastTRBtoTransferEventTRB(struct TRB* event_trb)
{
}

union PortStatusChangeEventTRB* CastTRBToPortStatusChangeEventTRB(struct TRB* event_trb);
{
}

union CommandCompletionEventTRB* CastTRBToCommandCompletionEventTRB(struct TRB* event_trb);
{
}
