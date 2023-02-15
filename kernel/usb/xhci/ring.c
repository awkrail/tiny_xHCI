#include "ring.h"

struct TRB* ReadDequeuePointer(struct EventRing *event_ring)
{
  return (struct TRB*)(interrupter->ERDP.bits.event_ring_dequeue_pointer << 4);
}

struct TRB* EventRingFront(struct EventRing *event_ring)
{
  return ReadDequeuePointer(event_ring);
}

bool HasEventRingFront(struct EventRing *event_ring)
{
  return EventRingFront(event_ring)->bits.cycle_bit == event_ring->cycle_bit;
}
