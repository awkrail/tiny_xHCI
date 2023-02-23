#include "ring.h"

union TRB* ReadDequeuePointer(struct EventRing *event_ring)
{
  return (union TRB*)(event_ring->interrupter->ERDP.bits.event_ring_dequeue_pointer << 4);
}

union TRB* EventRingFront(struct EventRing *event_ring)
{
  return ReadDequeuePointer(event_ring);
}

bool HasEventRingFront(struct EventRing *event_ring)
{
  return EventRingFront(event_ring)->bits.cycle_bit == event_ring->cycle_bit;
}

void Pop(struct EventRing *event_ring)
{
}
