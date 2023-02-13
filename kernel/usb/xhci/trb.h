#pragma once

#include <stdint.h>

union TRB
{
  uint32_t data[4];
  struct
  {
    uint64_t parameter;
    uint32_t status;
    uint32_t cycle_bit: 1;
    uint32_t evaluate_next_trb: 1;
    uint32_t : 8;
    uint32_t trb_type: 6;
    uint32_t control: 16;
  } __attribute__((packed)) bits;
};
