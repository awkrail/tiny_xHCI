#pragma once

#include <stddef.h>
#include <stdint.h>

#include "error.h"

struct DeviceManager
{
  size_t max_slot;
};

InitializeDevMgr(struct DeviceManager *devmgr,
                 size_t max_slot);
