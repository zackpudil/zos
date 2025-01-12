#ifndef PCI_H
#define PCI_H

#define PCI_E           0x80000000
#define PCI_REG_ADDRESS 0x0CF8
#define PCI_REG_DATA    0x0CFC

#include "types.h"

typedef struct { 
  u8 bus;
  u8 slot;
  u16 vendor_id;
  u16 device_id;
  u16 status;
  u16 command;
  u8 class_code;
  u8 sub_class;
  u8 header_type;
  u32 base_address_0;
  u32 base_address_1;
  u32 base_rom_address;
  u8 int_pin;
  u8 int_line;
} pci_device;

pci_device **get_all_devices();
void enable_bus_mastering(pci_device *device);

#endif
