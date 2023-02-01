#pragma once

#include <stdint.h>

extern void IoOut32(uint16_t addr, uint32_t data);
extern uint32_t IoIn32(uint16_t addr);
