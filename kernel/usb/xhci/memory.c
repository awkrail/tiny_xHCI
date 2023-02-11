#include <usb/memory.h>
#include <stdalign.h>
#include <stdint.h>

alignas(64) uint8_t memory_pool[kMemoryPoolSize];
volatile uintptr_t alloc_ptr = (uintptr_t)memory_pool;

uintptr_t CeilPtr(uintptr_t value, unsigned int alignment)
{
  return (value + alignment - 1) & ~(uintptr_t)(alignment - 1);
}

void* AllocMem(size_t size, unsigned int alignment, unsigned int boundary)
{
  if(alignment > 0)
    alloc_ptr = CeilPtr(alloc_ptr, boundary);

  if(boundary > 0) {
    unsigned int next_boundary = CeilPtr(alloc_ptr, boundary);
    if(next_boundary < alloc_ptr + size)
      alloc_ptr = next_boundary;
  }

  if((uintptr_t)memory_pool + kMemoryPoolSize < alloc_ptr + size)
    return NULL;

  uintptr_t p = alloc_ptr;
  alloc_ptr += size;
  return (void*)p;
}

Device**  AllocDeviceArray(size_t num_obj, unsigned int alignment, unsigned int boundary)
{
  return (Device**)AllocMem(sizeof(Device*) * num_obj, alignment, boundary);
}

DeviceContext** AllocDeviceContextArray(size_t num_obj, unsigned int alignment, unsigned int boundary)
{
  return (DeviceContext**)AllocMem(sizeof(DeviceContext*) * num_obj, alignment, boundary);
}

void FreeMem
