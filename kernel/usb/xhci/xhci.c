#include "logger.h"
#include "usb/xhci/xhci.h"

void InitializeController(struct Controller *xhc,
                          uintptr_t mmio_base)
{
  xhc->mmio_base = mmio_base;
  xhc->cap = (CapabilityRegisters*)mmio_base;
  xhc->op = (OperationalRegisters*)mmio_base; // + cap + ...
  xhc->max_ports = (uint8_t)cap.HCSPARAMS1; // Read.bits().max_ports..
}

enum Error InitializeXhci()
{
  enum Error err = InitializeDevMgr(devmgr, kDeviceSize);
  if(err)
    return err;
}
