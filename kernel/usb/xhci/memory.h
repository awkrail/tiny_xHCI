#pragma once

#include <stddef.h>
#include "device.h"

#define kMemoryPoolSize 131072 // 4096*32

// Allocate memory
struct USBDevice** AllocDeviceArray(size_t size, unsigned int alignment, unsigned int boundary);
struct USBDeviceContext** AllocDeviceContextArray(size_t num_obj, unsigned int alignment, unsigned int boundary);
union TRB* AllocTRBArray(size_t num_obj, unsigned int alignment, unsigned int boundary);

void* AllocMem(size_t size, unsigned int alignment, unsigned int boundary);

void FreeMem(void* p);
