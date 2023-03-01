#pragma once 

#include <stdint.h>

#include "../../error.h"
#include "memory.h"
#include "registers.h"
#include "trb.h"

struct Ring
{
  union TRB *buf;
  size_t buf_size;
  bool cycle_bit;
  size_t write_index;
};

union EventRingSegmentTableEntry
{
  uint32_t data[4];
  struct
  {
    uint64_t ring_segment_base_address;
    uint32_t ring_segment_size: 16;
    uint32_t : 16;
    uint32_t : 32;
  } __attribute__((packed)) bits;
};


struct EventRing
{
  union TRB *buf;
  size_t buf_size;
  bool cycle_bit;
  union EventRingSegmentTableEntry *erst;
  struct InterrupterRegisterSet *interrupter;
};

union TRB* ReadDequeuePointer(struct EventRing *event_ring);
union TRB* EventRingFront(struct EventRing *event_ring);
bool HasEventRingFront(struct EventRing *event_ring);

// Command ring functions
void CopyToLast(struct Ring *command_ring, uint32_t data[4]);
union TRB* PushCommandRing(struct Ring *command_ring, uint32_t data[4]);



void Pop(struct EventRing *event_ring);
