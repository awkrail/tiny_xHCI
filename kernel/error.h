#pragma once

enum Error {
  kSuccess,
  kFull,
  kEmpty,
  kNoEnoughMemory,
  kIndexOutOfRange,
  kHostControllerNotHalted,
  kInvalidSlotID,
  kPortNotConnected,
  kInvalidEndpointNumber,
  kTransferRingNotSet,
  kAlreadyAllocated,
  kNotImplemented,
  kInvalidDescriptor,
  kBufferTooSmall,
  kUnknownDevice,
  kNoCorrespondingSetupStage,
  kTransferFailed,
  kInvalidPhase,
  kUnknownXHCISpeedID,
  kNoWaiter,
  kLastOfCode,
};

const char *GetErrName(enum Error err);
