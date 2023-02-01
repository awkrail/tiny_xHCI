#pragma once

union HCSPARAMS1_Bitmap 
{
  uint32_t data[1];
  struct 
  {
    uint32_t max_device_slots: 8;
    uint32_t max_interrupters: 11;
    uint32_t padding: 5;
    uint32_t max_ports: 8;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union HCSPARAMS2_Bitmap 
{
  uint32_t data[1];
  struct 
  {
    uint32_t isochronous_scheduling_threshold: 4;
    uint32_t event_ring_segment_table_max: 4;
    uint32_t padding: 13;
    uint32_t max_scratchpad_buffers_high: 5;
    uint32_t scratchpad_restore: 1;
    uint32_t max_scratchpad_buffers_low: 5;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union HCSPARAMS3_Bitmap 
{
  uint32_t data[1];
  struct
  {
    uint32_t u1_device_eixt_latency: 8;
    uint32_t padding: 8;
    uint32_t u2_device_eixt_latency: 16;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union HCCPARAMS1_Bitmap
{
  uint32_t data[1];
  struct 
  {
    uint32_t addressing_capability_64: 1;
    uint32_t bw_negotiation_capability: 1;
    uint32_t context_size: 1;
    uint32_t port_power_control: 1;
    uint32_t port_indicators: 1;
    uint32_t light_hc_reset_capability: 1;
    uint32_t latency_tolerance_messaging_capability: 1;
    uint32_t no_secondary_sid_support: 1;
    uint32_t parse_all_event_data: 1;
    uint32_t stopped_short_packet_capability: 1;
    uint32_t stopped_edtla_capability: 1;
    uint32_t contiguous_frame_id_capability: 1;
    uint32_t maximum_primary_stream_array_size: 4;
    uint32_t xhci_extended_capabilities_pointer: 16;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union DBOFF_Bitmap
{
  uint32_t data[1];
  struct
  {
    uint32_t padding: 2;
    uint32_t doorbell_array_offset: 30;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union RTSOFF_Bitmap
{
  uint32_t data[1];
  struct
  {
    uint32_t padding: 5;
    uint32_t runtime_register_space_offset: 27;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union HCCPARAMS2_Bitmap
{
  uint32_t data[1];
  struct
  {
    uint32_t u3_entry_capability: 1;
    uint32_t configure_endpoint_command_max_exit_latency_too_large_capability: 1;
    uint32_t force_save_context_capability: 1;
    uint32_t compliance_transition_capability: 1;
    uint32_t large_esit_payload_capability: 1;
    uint32_t configuration_information_capability: 1;
    uint32_t padding: 26;
  } __attribute__((packed)) bits;
} __attribute__((packed));

struct CapabilityRegisters 
{
  /**
  MemMapRegister<DefaultBitmap<uint8_t>> CAPLENGTH;
  MemMapRegister<DefaultBitmap<uint8_t>> reserved1;
  MemMapRegister<DefaultBitmap<uint16_t>> HCIVERSION;
  MemMapRegister<HCSPARAMS1_Bitmap> HCSPARAMS1;
  MemMapRegister<HCSPARAMS2_Bitmap> HCSPARAMS2;
  MemMapRegister<HCSPARAMS3_Bitmap> HCSPARAMS3;
  MemMapRegister<HCCPARAMS1_Bitmap> HCCPARAMS1;
  MemMapRegister<DBOFF_Bitmap> DBOFF;
  MemMapRegister<RTSOFF_Bitmap> RTSOFF;
  MemMapRegister<HCCPARAMS2_Bitmap> HCCPARAMS2;
  **/
  uint32_t data[1];
} __attribute__((packed));

union USBCMD_Bitmap 
{
  uint32_t data[1];
  struct 
  {
    uint32_t run_stop: 1;
    uint32_t host_controller_reset: 1;
    uint32_t interrupter_enable: 1;
    uint32_t host_system_error_enable: 1;
    uint32_t padding1: 3;
    uint32_t lignt_host_controller_reset: 1;
    uint32_t controller_save_state: 1;
    uint32_t controller_restore_state: 1;
    uint32_t enable_wrap_event: 1;
    uint32_t enable_u3_mfindex_stop: 1;
    uint32_t stopped_short_packet_enable: 1;
    uint32_t cem_enable: 1;
    uint32_t padding2: 18;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union USBSTS_Bitmap {
  uint32_t data[1];
  struct 
  {
    uint32_t host_controller_halted: 1;
    uint32_t padding1: 1;
    uint32_t host_system_error: 1;
    uint32_t event_interrupt: 1;
    uint32_t port_change_detect: 1;
    uint32_t padding2: 3;
    uint32_t save_state_status: 1;
    uint32_t restore_state_status: 1;
    uint32_t save_restore_error: 1;
    uint32_t controller_not_ready: 1;
    uint32_t host_controller_error: 1;
    uint32_t padding3: 19;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union CRCR_Bitmap {
  uint64_t data[1];
  struct 
  {
    uint64_t ring_cycle_state: 1;
    uint64_t command_stop: 1;
    uint64_t command_abort: 1;
    uint64_t command_ring_running: 1;
    uint64_t padding: 2;
    uint64_t command_ring_pointer: 58;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union DCBAAP_Bitmap {
  uint64_t data[1];
  struct 
  {
    uint64_t padding: 6;
    uint64_t device_context_base_address_array_pointer: 26;
  } __attribute__((packed)) bits;
} __attribute__((packed));

union CONFIG_Bitmap {
  uint32_t data[1];
  struct 
  {
    uint32_t max_device_slots_enabled: 8;
    uint32_t u3_entry_enable: 1;
    uint32_t configuration_information_enable: 1;
    uint32_t padding: 22;
  } __attribute__((packed)) bits;
} __attribute__((packed));

struct OperationalRegisters {
  /**
  MemMapRegister<USBCMD_Bitmap> USBCMD;
  MemMapRegister<USBSTS_Bitmap> USBSTS;
  MemMapRegister<DefaultBitmap<uint32_t>> PAGESIZE;
  uint32_t reserved1[2];
  MemMapRegister<DefaultBitmap<uint32_t>> DNCTRL;
  MemMapRegister<CRCR_Bitmap> CRCR;
  uint32_t reserved2[4];
  MemMapRegister<DCBAAP_Bitmap> DCBAAP;
  MemMapRegister<CONFIG_Bitmap> CONFIG;
  **/
} __attribute__((packed));
