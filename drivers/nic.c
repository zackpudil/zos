#include "nic.h"
#include "../lib/mem.h"

static u32 base_address;
static u8 *rx_descs;
static u8 *tx_descs;

static u8 rx_cur;
static u8 tx_cur;

void write_command(u32 addr, u32 value) {
  u32 laddr = base_address + addr;
  (*((volatile u32 *)(laddr))) = value;
}

u32 read_command(u32 addr) {
  u32 laddr = base_address + addr;
  return *((volatile u32*)(laddr));
}

bool detect_eeprom() {
  u32 val = 0;
  write_command(0x0014, 0x1);

  for(int i = 0; i < 1000;  i++) {
    val = read_command(0x0014);
    if (val & 0x10) return true;
  }

  return false;
}

u32 eeprom_read(u8 addr) {
  u16 data = 0;
  u32 temp = 0;

  write_command(0x0014, ((u32)(addr) << 8) | 1);

  while ( !((temp = read_command(0x0014)) & (1 << 4)) );

  data = (u16)((temp >> 16) & 0xFFFF);

  return data;
}

void rx_tx_init() {

  rx_descs = (u8 *)malloc(16*32 + 16, true, 0);
  tx_descs = (u8 *)malloc(16*8 + 16, true, 0);

  for(u8 i = 0; i < 32; i++) {

    rx_desc *rx = (rx_desc *)(rx_descs + i*16);
    rx->addrl = malloc(8192 + 16, false, 0);
    rx->addrh = 0;
    rx->status = 0;

    if (i < 8) {
      tx_desc *tx = (tx_desc *)(tx_descs + i*16);
      tx->addrl = 0;
      tx->addrh = 0;
      tx->cmd = 0;
      tx->status = 0x1;
    }
  }

  write_command(0x2800, (u32)rx_descs);
  write_command(0x2804, 0);
  write_command(0x2808, 32*16);
  write_command(0x2810, 0);
  write_command(0x2818, 31);
  write_command(0x0100, 0x202803E);
  rx_cur = 0;

  write_command(0x3800, (u32)tx_descs);
  write_command(0x3804, 0);
  write_command(0x3808, 8*16);
  write_command(0x3810, 0);
  write_command(0x3818, 0);
  write_command(0x0400, 0x10400FA);
  write_command(0x0410, 0xA0280A);
  tx_cur = 0;
}

void nic_enable_interrupt() {
  write_command(0x00D0, 0x1F6DC);
  write_command(0x00D0, 0xff & ~4);
  read_command(0xC0);
}

u8 *get_mac_address(pci_device *device) {
  u8 *mac = (u8 *)malloc(sizeof(u8)*6, true, 0);

  u32 temp;
  for(u8 i = 0; i < 3; i++) {
    temp = eeprom_read(i);
    mac[i*2] = temp & 0xff;
    mac[i*2 + 1] = temp >> 8;
  }

  return mac;
}

u32 nic_handle_interrupt() {
  u32 status = read_command(0x00C0);

  return status;
}

u8 *init_nic(pci_device *device) {
  base_address = device->base_address_0;
  enable_bus_mastering(device);

  write_command(0x0100, 0);
  write_command(0x0400, 0);
  read_command(0x0080);

  u32 ctrl = read_command(0x0000);
  ctrl |= (1 << 26);
  write_command(0x0000, ctrl);

  while (read_command(0x000) & (1 << 26)) {};

  write_command(0x0010, 0x07);
  u8 *mac = get_mac_address(device);

  write_command(0x0028, 0);
  write_command(0x002C, 0);
  write_command(0x0030, 0);
  write_command(0x0170, 0);

  write_command(0x0000, 0x60);

  rx_tx_init();

  write_command(0x282C, 0);
  write_command(0x2820, 1 << 31);
  write_command(0x00C4, 5000);

  read_command(0x0008);

  return mac;
}

rx_desc *nic_read_packet() {
  rx_desc *ret = (rx_desc *)(rx_descs + 16*rx_cur);

  write_command(0x2818, rx_cur);
  rx_cur = (rx_cur + 1) % 32;

  return ret;
}

void nic_send_packet(ethernet_packet *p_data, u16 p_len) {
  tx_desc *current = (tx_desc *)(tx_descs + 16*tx_cur);
  current->addrl = (u64)p_data;
  current->length = p_len + 10;
  current->cmd = 0b10011011;
  current->status = 0;

  tx_cur = (tx_cur + 1) % 8;

  asm volatile("cli");

  write_command(0x3818, tx_cur);
  while (current->status == 0) {}

  rx_desc *rx = (rx_desc *)rx_descs;
  u8 t = tx_cur;
  tx_cur = rx->status;
  tx_cur = t;

  asm volatile("sti");
}
