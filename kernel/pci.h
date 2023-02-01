#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "error.h"

struct ClassCode {
  uint8_t base, sub, interface;
};

struct Device {
  uint8_t bus, device, function, header_type;
  struct ClassCode class_code;
};

uint32_t shl32(uint32_t x, unsigned int bits);

// get/set registers
void WriteAddress(uint32_t address);
void WriteData(uint32_t value);
uint32_t ReadData();

// read PCI configuration settings
uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function);
uint16_t ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function);
uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function);
struct ClassCode ReadClassCode(uint8_t bus, uint8_t device, uint8_t function);
uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function);

// scan all devices
bool IsSingleFunctionDevice(uint8_t header_type);
enum Error ScanDevice(uint8_t bus, uint8_t device);
enum Error ScanFunction(uint8_t bus, uint8_t device, uint8_t function);
enum Error AddDevice(const struct Device dev);
enum Error ScanBus(uint8_t bus);
enum Error ScanAllBus();

// check if class code matches to all / base and sub / base
bool MatchAllClassCode(struct ClassCode class_code, 
                       uint8_t base, uint8_t sub,
                       uint8_t interface);
bool MatchBaseAndSubClassCode(struct ClassCode class_code, 
                              uint8_t base, uint8_t sub);
bool MatchBaseClassCode(struct ClassCode class_code,
                        uint8_t base);

const uint8_t CalcBarAddress(unsigned int bar_index);

uint32_t ReadConfReg(const struct Device *device, uint8_t reg_addr);

void WriteConfReg(const struct Device *device, uint8_t reg_addr, 
                  uint32_t value);

enum Error ReadBar(struct Device *device, uint64_t *bar, 
                   unsigned int bar_index);

struct Device devices[32];
int num_device;
