#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../../error.h"
#include "context.h"
#include "device.h"

struct DeviceManager
{
  size_t max_slots;
  struct Device **devices;
  struct DeviceContext **device_context_ptrs;
};

enum Error InitializeDevMgr(struct DeviceManager *devmgr,
                            size_t max_slots);
