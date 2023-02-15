#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "frame_buffer_config.h"
#include "graphics.h"
#include "font.h"
#include "console.h" 
#include "pci.h"
#include "error.h"
#include "logger.h"

// xHCI host driver
#include "usb/xhci/xhci.h"

struct Console console;

int printk(const char *format, ...)
{
  va_list ap;
  int result;
  char s[1024];

  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap);

  PutString(&console, s);
  return result;
}

void SwitchEhci2Xhci(const struct Device *xhc_dev)
{
  bool intel_ehc_exist = false;
  for(int i=0; i < num_device; ++i) {
    uint8_t bus = devices[i].bus;
    uint8_t device = devices[i].device;
    uint8_t function = devices[i].function;
    struct ClassCode class_code =devices[i].class_code;

    if(MatchAllClassCode(class_code, 0x0cu, 0x03u, 0x20u)
      && 0x8086 == ReadVendorId(bus, device, function)) {
      intel_ehc_exist = true;
      break;
    }
  }
  
  Log(kDebug, &console, "intel EHC exist?: %d\n", intel_ehc_exist);

  if(!intel_ehc_exist)
    return;

  uint32_t superspeed_ports = ReadConfReg(xhc_dev, 0xdc);
  WriteConfReg(xhc_dev, 0xd8, superspeed_ports);
  uint32_t ehci2xhci_ports = ReadConfReg(xhc_dev, 0xd4);
  WriteConfReg(xhc_dev, 0xd0, ehci2xhci_ports);
  Log(kDebug, &console, "SwitchEhci2Xhci: SS = %02, xHCI = %02x\n",
      superspeed_ports, ehci2xhci_ports);
}

const struct PixelColor kDesktopBGColor = {128, 128, 128};
const struct PixelColor kDesktopFGColor = {255, 255, 255};

void KernelMain(const struct FrameBufferConfig *frame_buffer_config) 
{
  const struct PixelColor black = {0, 0, 0};
  const struct PixelColor white = {255, 255, 255};
  
  const struct IntVector2D ul_p = {0, 0};
  const struct IntVector2D lr_p = {frame_buffer_config->horizontal_resolution, 
                                   frame_buffer_config->vertical_resolution};

  InitConsole(&console, frame_buffer_config, &black, &white);
  FillRectangle(frame_buffer_config, &ul_p, &lr_p, &white);
  SetLogLevel(kWarn);

  // search xHC devices
  printk("Running ScanAllBus...\n");
  enum Error err = ScanAllBus();
  printk("ScanAllBus: %s\n", GetErrName(err));
  
  // search intel devices
  struct Device *xhc_dev = NULL;
  for(int i=0; i<num_device; ++i) {
    if(MatchAllClassCode(devices[i].class_code, 0x0cu, 0x03u, 0x30u)) {
      xhc_dev = &devices[i];
      uint8_t bus = xhc_dev->bus;
      uint8_t device = xhc_dev->device;
      uint8_t function = xhc_dev->function;
      
      if(0x8086 == ReadVendorId(bus, device, function))
        break;
    }
  }

  if(xhc_dev) {
    Log(kInfo, &console, "xHC has been found: %d.%d.%d\n",
        xhc_dev->bus, xhc_dev->device, xhc_dev->function);
  }

  // Load MMIO registers for xHCI device
  uint64_t xhc_bar;
  err = ReadBar(xhc_dev, &xhc_bar, 0);
  const uint64_t xhc_mmio_base = xhc_bar & ~(uint64_t)0xf;

  // Initialize xHCI controller
  struct Controller xhc;
  struct DeviceManager dev_mgr;
  err = InitializeController(&dev_mgr, &xhc, xhc_mmio_base);
  Log(kDebug, &console, "xHC Initialize: %s\n", GetErrName(err));

  for(int i = 1; i <= xhc.max_ports; ++i) {
    struct Port port = xHCIPortAt(&xhc, i);
    Log(kDebug, &console, "Port %d: IsConnected=%d\n", i, IsPortConnected(&port));
    
    if(IsPortConnected(&port)) {
      err = xHCIConfigurePort(&xhc, &port);
      Log(kDebug, &console, "Port configure errcode=%s\n", GetErrName(err));
    }
  }

  while (1) {
    err = xHCIProcessEvent(xhc);
    if(err) {
      Log(kError, "Error while ProcessEvent: %s at %s:%d\n",
          GetErrName(err), GetFileName(err), GetLineName(err));
    }
  }

  while (1) __asm__("hlt");
}
