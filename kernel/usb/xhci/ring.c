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

void CopyToLast(struct Ring *command_ring, uint32_t data[4])
{
  for(int i=0; i<3; ++i) {
    // index 0 to 2 must be written prior to data[3]
    command_ring->buf[command_ring->write_index].data[i] = data[i];
  }
  command_ring->buf[command_ring->write_index].data[3]
    = (data[3] & 0xfffffffeu) | (uint32_t)command_ring->cycle_bit;
}

union TRB* PushCommandRing(struct Ring *command_ring, uint32_t data[4])
{
  union TRB* trb_ptr = &command_ring->buf[command_ring->write_index];
  CopyToLast(command_ring, data);

  ++command_ring->write_index;
  if(command_ring->write_index == command_ring->buf_size - 1) {
    union LinkTRB link = InitializeLinkTRB(command_ring->buf);
    link.bits.toggle_cycle = true;
    CopyToLast(command_ring, link.data);

    command_ring->write_index = 0;
    command_ring->cycle_bit = !command_ring->cycle_bit;
  }

  return trb_ptr;
}

void Pop(struct EventRing *event_ring)
{
}
