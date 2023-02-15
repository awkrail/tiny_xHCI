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

union NormalTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct
  {
    uint64_t data_buffer_pointer;

    uint32_t trb_transfer_length: 17;
    uint32_t td_size: 5;
    uint32_t interrupter_target: 10;

    uint32_t cycle_bit: 1;
    uint32_t evaluate_next_trb: 1;
    uint32_t interrupt_on_short_packet: 1;
    uint32_t no_snoop: 1;
    uint32_t chain_bit: 1;
    uint32_t interrupt_on_completion: 1;
    uint32_t immediate_data: 1;
    uint32_t : 2;
    uint32_t block_event_interrupt: 1;
    uint32_t trb_type: 6;
    uint32_t : 16;
  } __attribute__((packed)) bits;
};

union SetupStageTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct 
  {
    uint64_t : 64;
    uint32_t : 22;
    uint32_t interrupter_target: 10;
    uint32_t cycle_bit: 1;
    uint32_t evaluate_next_trb: 1;
    uint32_t : 2;
    uint32_t chain_bit: 1;
    uint32_t interrupt_on_completion: 1;
    uint32_t : 4;
    uint32_t trb_type: 6;
    uint32_t direction: 1;
    uint32_t : 15;
  } __attribute__((packed)) bits;
};

union LinkTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct
  {
    uint64_t : 4;
    uint64_t ring_segment_pointer: 60;

    uint32_t : 22;
    uint32_t interrupter_target: 10;

    uint32_t cycle_bit: 1;
    uint32_t toggle_cycle: 1;
    uint32_t : 2;
    uint32_t chain_bit: 1;
    uint32_t interrupt_on_completion: 1;
    uint32_t : 4;
    uint32_t trb_type: 6;
    uint32_t : 16;
  } __attribute__((packed)) bits;
};

union NoOpTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct
  {
    uint64_t : 64;
    uint32_t : 22;
    uint32_t interrupter_target: 10;
    uint32_t cycle_bit: 1;
    uint32_t evaluate_next_trb: 1;
    uint32_t : 2;
    uint32_t chain_bit: 1;
    uint32_t interrupt_on_completion: 1;
    uint32_t : 4;
    uint32_t trb_type: 6;
    uint32_t : 16;
  } __attribute__((packed)) bits;
};

union EnableSlotCommandTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct
  {
    uint32_t: 32;
    uint32_t: 32;
    uint32_t: 32;

    uint32_t cycle_bit: 1;
    uint32_t : 9;
    uint32_t trb_type: 6;
    uint32_t slot_type: 5;
    uint32_t : 11;
  } __attribute__((packed)) bits;
};

union AddressDeviceCommandTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct
  {
    uint64_t : 4;
    uint64_t input_context_pointer: 60;

    uint32_t : 32;

    uint32_t cycle_bit: 1;
    uint32_t : 8;
    uint32_t block_set_address_request: 1;
    uint32_t trb_type: 6;
    uint32_t : 8;
    uint32_t slot_id: 8;
  } __attribute__((packed)) bits;
};

union ConfigureEndpointCommandTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct
  {
    uint64_t : 4;
    uint64_t input_context_pointer: 60;

    uint32_t : 32;
    uint32_t cycle_bit: 1;
    uint32_t : 8;
    uint32_t deconfigure: 1;
    uint32_t trb_type: 6;
    uint32_t : 8;
    uint32_t slot_id: 8;
  } __attribute__((packed)) bits;
};

union StopEndpointCommandTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct
  {
    uint32_t : 32;
    uint32_t : 32;
    uint32_t : 32;

    uint32_t cycle_bit: 1;
    uint32_t : 9;
    uint32_t trb_type: 6;
    uint32_t endpoint_id: 5;
    uint32_t : 2;
    uint32_t suspend: 1;
    uint32_t slot_id: 8;
  } __attribute__((packed)) bits;
};

union NoOpCommandTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct
  {
    uint32_t : 32;
    uint32_t : 32;
    uint32_t : 32;

    uint32_t cycle_bit: 1;
    uint32_t : 9;
    uint32_t trb_type: 6;
    uint32_t : 16;
  } __attribute__((packed)) bits;
};

union TransferEventTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct
  {
    uint64_t trb_pointer: 64;
    
    uint32_t trb_transfer_length: 24;
    uint32_t completion_code: 8;

    uint32_t cycle_bit: 1;
    uint32_t : 1;
    uint32_t event_data: 1;
    uint32_t : 7;
    uint32_t trb_type: 6;
    uint32_t endpoint_id: 5;
    uint32_t : 3;
    uint32_t slot_id: 8;
  } __attribute__((packed)) bits;
};

union CommandCompletionEventTRB
{
  unsigned int Type;
  uint32_t data[4];
  struct
  {
    uint64_t : 4;
    uint64_t command_trb_pointer: 60;

    uint32_t command_completion_parameter: 24;
    uint32_t completion_code:8;

    uint32_t cycle_bit: 1;
    uint32_t : 9;
    uint32_t trb_type: 6;
  } __attribute__((packed)) bits;
};

// dynamic cast from event_trb to other types
union TransferEventTRB* CastTRBtoTransferEventTRB(union TRB* event_trb);
union PortStatusChangeEventTRB* CastTRBToPortStatusChangeEventTRB(union TRB* event_trb);
union CommandCompletionEventTRB* CastTRBToCommandCompletionEventTRB(union TRB* event_trb);
