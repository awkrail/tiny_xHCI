#pragma once

#include <stddef.h>
//#include <>

#define kMemoryPoolSize 131072 // 4096*32

// Allocate memory
Device** AllocDeviceArray(size_t size, unsigned int alignment, unsigned int boundary);
DeviceContext** AllocDeviceContextArray(size_t size, unsigned int alignment, unsigned int boundary);


void* AllocMem(size_t size, unsigned int alignment, unsigned int boundary);
void* AllocArray(size_t num_obj, unsigned int alignment, unsigned int boundary);



