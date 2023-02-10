#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../../error.h"
#include "../../logger.h"
#include "devmgr.h"
#include "registers.h"

#define kDeviceSize 8

struct Controller
{
  uintptr_t mmio_base;
  volatile struct CapabilityRegisters *cap;
  volatile struct OperationalRegisters *op;
};

struct CapabilityRegisters
{
  uint32_t CAPLENGTH_HCIVERSION;
  union HCSPARAMS1_Bitmap HCSPARAMS1;
  union HCSPARAMS2_Bitmap HCSPARAMS2;
  union HCSPARAMS3_Bitmap HCSPARAMS3;
  union HCCPARAMS1_Bitmap HCCPARAMS1;
  union DBOFF_Bitmap DBOFF;
  union RTSOFF_Bitmap RTSOFF;
  union HCCPARAMS2_Bitmap HCCPARAMS2;
} __attribute__((packed));

struct OperationalRegisters
{
  union USBCMD_Bitmap USBCMD;
  union USBSTS_Bitmap USBSTS;
  uint32_t PAGESIZE;
  uint32_t reserved1[2];
  uint32_t DNCTRL;
  union CRCR_Bitmap CRCR;
  uint32_t reserved2[4];
  union DCBAAP_Bitmap DCBAAP;
  union CONFIG_Bitmap CONFIG;
} __attribute__((packed));

// Controller-related functions
void InitializeController(struct DeviceManager *dev_mgr,
                          struct Controller *xhc,
                          uintptr_t mmio_base,
                          struct Console *console);
void PrintAllRegisters(struct Controller *xhc,
                       struct Console *console);
void SetCapAndOpRegisters(struct Controller *xhc,
                          struct Console *console);
void ResetController(struct Controller *xhc,
                     struct Console *console);
void SetMaxSlotEnabled(struct Controller* xhc,
                       struct Console *console);

// Register-related functions
uint8_t ReadCAPLENGTH(volatile struct CapabilityRegisters *cap);
uint16_t ReadHCIVERSION(volatile struct CapabilityRegisters *cap);
