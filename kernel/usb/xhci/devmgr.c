#include "devmgr.h"
#include "memory.h"

enum Error InitializeDevMgr(struct DeviceManager *devmgr,
                            size_t max_slots)
{
  devmgr->max_slots = max_slots;

  devmgr->devices = AllocDeviceArray(max_slots + 1, 0, 0);
  if(devmgr->devices == NULL)
    return kNoEnoughMemory;

  devmgr->device_context_ptrs = AllocDeviceContextArray(max_slots + 1, 64, 4096);
  if(devmgr->device_context_ptrs == NULL) {
    FreeMem(devmgr->devices);
    return kNoEnoughMemory;
  }

  // Initialize device context
  for(size_t i = 0; i <= max_slots; ++i) {
    devmgr->devices[i] = NULL;
    devmgr->device_context_ptrs[i] = NULL;
  }

  return kSuccess;
}
