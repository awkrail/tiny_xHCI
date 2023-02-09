#pragma once

#include <stddef>
#include <stdint>

#include "../../error.h"
//#include "context.h"
//#include "device.h"

struct DeviceManager
{
  size_t max_slots;
  Device** devices;
  DeviceContext** device_context_ptrs;
};

Error Initialize(size_t max_slots);
