#include "error.h"

const char *GetErrName(enum Error err)
{
  switch(err) {
    case kSuccess:
      return "kSuccess";
      break;
    
    case kFull:
      return "kFull";
      break;

    case kEmpty:
      return "kEmpty";
      break;

    case kNoEnoughMemory:
      return "kNoEnoughMemory";
      break;

    case kIndexOutOfRange:
      return "kIndexOutOfRange";
      break;

    case kHostControllerNotHalted:
      return "kHostControllerNotHalted";
      break;

    case kInvalidSlotID:
      return "kInvalidSlotID";
      break;

    case kPortNotConnected:
      return "kPortNotConnected";
      break;
    
    case kInvalidEndpointNumber:
      return "kInvalidEndpointNumber";
      break;

    case kTransferRingNotSet:
      return "kTransferRingNotSet";
      break;

    case kAlreadyAllocated:
      return "kAlreadyAllocated";
      break;

    case kNotImplemented:
      return "kNotImplemented";
      break;

    case kInvalidDescriptor:
      return "kInvalidDescriptor";
      break;

    case kBufferTooSmall:
      return "kBufferTooSmall";
      break;

    case kUnknownDevice:
      return "kUnknownDevice";
      break;

    case kNoCorrespondingSetupStage:
      return "kNoCorrespondingSetupStage";
      break;

    case kTransferFailed:
      return "kTransferFailed";
      break;

    case kInvalidPhase:
      return "kInvalidPhase";
      break;

    case kUnknownXHCISpeedID:
      return "kUnknownXHCISpeedID";
      break;

    case kNoWaiter:
      return "kNoWaiter";
      break;

    case kLastOfCode:
      return "kLastOfCode";
      break;

    default:
      return "kErrorWithNotInList";
      break;
  }
}
