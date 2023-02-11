#pragma once

//#include "endpoint.h"

union SlotContext 
{
  uint32_t dwords[8];
  struct
  {
    uint32_t route_string: 20;
    uint32_t speed: 4;
    uint32_t : 1;
    uint32_t mtt: 1;
    uint32_t hub: 1;
    uint32_t context_entries: 5;

    uint32_t max_exit_latency: 16;
    uint32_t root_hub_port_num: 8;
    uint32_t num_ports: 8;

    // TT: Transaction Translator
    uint32_t tt_hub_slot_id: 8;
    uint32_t tt_port_num: 8;
    uint32_t ttt: 2;
    uint32_t : 4;
    uint32_t interrupt_target: 10;
    
    uint32_t usb_device_address: 8;
    uint32_t : 19;
    uint32_t slot_state: 5;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union EndPointContext
{
  uint32_t dwords[8];
  struct
  {
    uint32_t ep_state: 3;
    uint32_t : 5;
    uint32_t mult: 2;
    uint32_t max_primary_streams: 5;
    uint32_t linear_stream_array: 1;
    uint32_t interval: 8;
    uint32_t max_esit_payload_hi: 8;

    uint32_t : 1;
    uint32_t error_count: 2;
    uint32_t ep_type: 3;
    uint32_t : 1;
    uint32_t host_initiate_disable: 1;
    uint32_t max_burst_size: 8;
    uint32_t max_packet_size: 16;

    uint32_t dequeue_cycle_state: 1;
    uint32_t : 3;
    uint64_t tr_dequeue_pointer: 60;

    uint32_t average_trb_length: 16;
    uint32_t max_esit_payload_lo: 16;
  } __attribute__((packed));
} __attribute__((packed));

struct USBDeviceContext
{
  union SlotContext slot_context;
  union EndPointContext ep_contexts[31];
} __attribute__((packed));
