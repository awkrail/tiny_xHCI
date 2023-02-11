#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../error.h"
#include "context.h"
#include "registers.h"
//#include "trb.h"

enum State
{
  kInvalid,
  kBlank,
  kSlotAssigning,
  kSlotAssigned
};


struct USBDevice 
{
  //ClassDriver *class_driver[16];
  uint8_t buf[256];

  uint8_t num_configuration;
  uint8_t config_index;

  bool is_initialized; // false, default
  int initialize_phase; // 0, default
  int num_ep_configs;
  // ArrayMap<SetupData, ClassDriver*, 4> event_waiters;

  _Alignas(64) struct USBDeviceContext ctx;
  //alignas(64) struct InputContext input_ctx;

  uint8_t slot_id;
  //struct DoorbellRegister *dbreg;

  enum State state;
  //Ring *transfer_rings[31];
  // arraymap: TODO
  // ArrayMap<const void*, const SetupStageTRB*, 16> setup_stage_map;
};
