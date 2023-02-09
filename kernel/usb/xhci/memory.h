#pragma once

#include <stddef.h>

#define kMemoryPoolSize 131072 // 4096*32

void* AllocMem(size_t size, unsigned int alignment, unsigned int boundary);
void* AllocArray(size_t num_obj, unsigned int alignment, unsigned int boundary);



