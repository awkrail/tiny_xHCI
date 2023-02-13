#include <stdint.h>
#include "../../error.h"
#include "memory.h"
#include "registers.h"
#include "trb.h"

struct Ring
{
  struct TRB *buf;
  size_t buf_size;
  bool cycle_bit;
  size_t write_index;
};

struct EventRing
{
  struct TRB *buf;
  size_t buf_size;
  bool cycle_bit;
  EventRingSegmentTableEntry erst;
  InterrupterRegisterSet *interrupter;
};
