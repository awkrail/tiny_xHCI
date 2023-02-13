#include "memory.h"
#include <stdint.h>

_Alignas(64) uint8_t memory_pool[kMemoryPoolSize];
uintptr_t alloc_ptr = (uintptr_t)memory_pool;

uintptr_t CeilPtr(uintptr_t value, unsigned int alignment)
{
  return (value + alignment - 1) & ~(uintptr_t)(alignment - 1);
}

void* AllocMem(size_t size, unsigned int alignment, unsigned int boundary)
{
  if(alignment > 0)
    alloc_ptr = CeilPtr(alloc_ptr, alignment);

  if(boundary > 0) {
    uintptr_t  next_boundary = CeilPtr(alloc_ptr, boundary);
    if(next_boundary < alloc_ptr + size)
      alloc_ptr = next_boundary;
  }

  if((uintptr_t)memory_pool + kMemoryPoolSize < alloc_ptr + size)
    return NULL;

  uintptr_t p = alloc_ptr;
  alloc_ptr += size;
  return (void*)p;
}

struct USBDevice**  AllocDeviceArray(size_t num_obj, unsigned int alignment, unsigned int boundary)
{
  return (struct USBDevice**)AllocMem(sizeof(struct Device*) * num_obj, alignment, boundary);
}

struct USBDeviceContext** AllocDeviceContextArray(size_t num_obj, unsigned int alignment, unsigned int boundary)
{
  return (struct USBDeviceContext**)AllocMem(sizeof(struct USBDeviceContext*) * num_obj, alignment, boundary);
}

void FreeMem(void* p) {};
