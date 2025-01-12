#include "pci.h"
#include "ports.h"
#include "../lib/mem.h"

u16 pci_config_read(u8 bus, u8 device, u8 func, u8 offset) {
  u32 address;
  u32 lbus = (u32)bus;
  u32 ldev = (u32)device;
  u32 lfunc = (u32)func;
  u16 temp = 0;

  address = (lbus << 16) | (ldev << 11) | (lfunc << 8) | (offset & 0xFC) | PCI_E;

  port_dword_out(PCI_REG_ADDRESS, address);

  u32 res = port_dword_in(PCI_REG_DATA);
  temp = (u16)(res >> ((offset & 2) * 8));

  return temp;
}

void enable_bus_mastering(pci_device *device) {
  device->command = device->command | 4;

  u32 address;
  u32 lbus = (u32)device->bus;
  u32 ldev = (u32)device->slot;
  u32 lfunc = 0;

  address = (lbus << 16) | (ldev << 11) | (lfunc << 8) | (4 & 0xFC) | PCI_E;

  port_dword_out(PCI_REG_ADDRESS, address);
  port_dword_out(PCI_REG_DATA, (u32)(device->status << 16 ) | device->command);
}

pci_device **get_all_devices() {
  pci_device **devices = (pci_device **)malloc((sizeof(pci_device *)*5), true, 0);
  u8 pci_idx = 0;

  for(u16 bus = 0; bus < 1; bus++) {
    for(u8 dev = 0; dev < 5; dev++) {
      u16 vendor_id = pci_config_read(bus, dev, 0, 0);
      if (vendor_id == 0xFFFF) continue;

      u16 device_id = pci_config_read(bus, dev, 0, 2);
      if (device_id == 0xFFFF) continue;

      u16 command = pci_config_read(bus, dev, 0, 4);
      u16 status = pci_config_read(bus, dev, 0, 6);
      u16 class_sub = pci_config_read(bus, dev, 0, 10);
      u16 bist_header = pci_config_read(bus, dev, 0, 14);

      u16 base_address_0_low = pci_config_read(bus, dev, 0, 16);
      u16 base_address_0_high = pci_config_read(bus, dev, 0, 18);

      u16 base_address_1_low = pci_config_read(bus, dev, 0, 20);
      u16 base_address_1_high = pci_config_read(bus, dev, 0, 22);

      u16 base_address_rom_low = pci_config_read(bus, dev, 0, 48);
      u16 base_address_rom_high = pci_config_read(bus, dev, 0, 50);

      u16 int_line_pin = pci_config_read(bus, dev, 0, 60);

      pci_device *device = (pci_device *)malloc(sizeof(pci_device), true, 0);
      device->bus = bus;
      device->slot = dev;
      device->vendor_id = vendor_id;
      device->device_id = device_id;
      device->status = status;
      device->command = command;
      device->class_code = (u8)(class_sub >> 8);
      device->sub_class = (u8)(class_sub & 0x00ff);
      device->header_type = (u8)(bist_header & 0x00ff);
      device->base_address_0 = (u32)( (base_address_0_high << 16) + base_address_0_low);
      device->base_address_1 = (u32)( (base_address_1_high << 16) + base_address_1_low);
      device->base_rom_address = (u32)( (base_address_rom_high << 16) + base_address_rom_low);
      device->int_pin = (u8)(int_line_pin >> 8);
      device->int_line = (u8)(int_line_pin & 0x00ff);

      devices[pci_idx] = device;
      pci_idx++;
    }
  }

  return devices;
}


