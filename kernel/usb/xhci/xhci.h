#pragma once

#include <stddef.h>
#include <stdint.h>

struct Controller
{
  uintptr_t mmio_base;
  struct CapabilityRegisters *cap;
  struct OperationalRegisters *op;
  //uint8_t max_ports;
};

struct CapabilityRegisters
{
  uint32_t CAPLENGTH_HCIVERSION;
  uint32_t HCSPARAMS1;
  uint32_t HCSPARAMS2;
  uint32_t HCSPARAMS3;
  uint32_t HCCPARAMS1;
  uint32_t DBOFF;
  uint32_t RTSOFF;
  uint32_t HCCPARAMS2;
} __attribute__((packed));

struct OperationalRegisters
{
  uint32_t USBCMD;
  uint32_t USBSTS;
  uint32_t PAGESIZE;
  uint32_t reserved1[2];
  uint32_t DNCTRL;
  uint64_t CRCR;
  uint32_t reserved2[4];
  uint64_t DCBAAP;
  uint32_t CONFIG;
} __attribute__((packed));

void InitializeController(struct Controller *xhc,
                          uintptr_t mmio_base);

uint8_t ReadCAPLENGTH(const struct CapabilityRegisters *cap);
uint16_t ReadHCIVERSION(const struct CapabilityRegisters *cap);
