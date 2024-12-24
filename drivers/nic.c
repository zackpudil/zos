#include "nic.h"
#include "../lib/mem.h"
#include "../interrupts/isr.h"

static u32 base_address;

static rx_desc *rx_descs[32];
static u8 rx_cur = 0;

static tx_desc *tx_descs[8];
static u8 tx_cur = 0;

static u8 *mac;

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

  u8 *rx_ptr = (u8 *)malloc(sizeof(rx_desc)*32 + 16, true, 0);
  u8 *tx_ptr = (u8 *)malloc(sizeof(tx_descs)*8 + 16, true, 0);

  for(u8 i = 0; i < 32; i++) {

    rx_descs[i] = (rx_desc *)(rx_ptr + i*16);
    rx_descs[i]->addr = (u64)malloc(8192 + 16, false, 0);
    rx_descs[i]->status = 0;

    if (i < 8) {
      tx_descs[i] = (tx_desc *)(tx_ptr + i*16);
      tx_descs[i]->addr = 0;
      tx_descs[i]->cmd = 0;
      tx_descs[i]->status = 0x1;
    }
  }

  write_command(0x2800, (u32)rx_ptr);
  write_command(0x2804, 0);

  write_command(0x2808, 32*16);
  write_command(0x2810, 0);
  write_command(0x2818, 31);

  
  write_command(0, 0x603801E);

  write_command(0x3800, (u32)tx_ptr);
  write_command(0x3804, 0);

  write_command(0x3808, 8*16);
  write_command(0x3810, 0);
  write_command(0x3818, 0);
  write_command(0x0400, 0x010400FA);
}

void enable_interrupt() {
  write_command(0x00D0, 0x1F6DC);
  write_command(0x00D0, 0xFF & ~4);
  read_command(0xC0);
}

void get_mac_address(pci_device *device) {
  mac = (u8 *)malloc(sizeof(u8)*6, true, 0);

  u32 temp;
  for(u8 i = 0; i < 3; i++) {
    temp = eeprom_read(i);
    mac[i*2] = temp & 0xff;
    mac[i*2 + 1] = temp >> 8;
  }
}

static void handle_interrupt(registers_t regs) {
  UNUSED(regs);

  write_command(0x00D0, 0x1);

  u32 status = read_command(0xC0);
  UNUSED(status);
}

u8 *init_network(pci_device *device) {
  UNUSED(rx_cur);
  base_address = device->base_address_0;
  enable_bus_mastering(device);
  detect_eeprom(device);
  get_mac_address(device);

  for(int i = 0; i < 0x80; i++) write_command(0x5200 + i*4, 0);

  register_interrupt_handler(IRQ0 + device->int_line, handle_interrupt);

  enable_interrupt();
  rx_tx_init();

  return mac;
}

void send_packet(void *p_data, u16 p_len) {
  tx_desc *current = tx_descs[tx_cur];
  current->addr = (u64)p_data;
  current->length = p_len;
  current->cmd = 0x0D;
  current->status = 0;
  tx_cur = (tx_cur + 1) % 8;
  write_command(0x3818, tx_cur);
  while (!(current->status & 0xff));
}
