#include "nic.h"
#include "../lib/mem.h"

static u32 base_address;
static u8 *rx_descs;
static u8 *tx_descs;

static u8 rx_cur;
static u8 tx_cur;

void write_register(u32 addr, u32 value) {
  u32 laddr = base_address + addr;
  (*((volatile u32 *)(laddr))) = value;
}

u32 read_register(u32 addr) {
  u32 laddr = base_address + addr;
  return *((volatile u32*)(laddr));
}

bool detect_eeprom() {
  u32 val = 0;
  write_register(EPROM_REG, 0x1);

  for(int i = 0; i < 1000;  i++) {
    val = read_register(EPROM_REG);
    if (val & 0x10) return true;
  }

  return false;
}

u32 eeprom_read(u8 addr) {
  u16 data = 0;
  u32 temp = 0;

  write_register(EPROM_REG, ((u32)(addr) << 8) | 1);

  while ( !((temp = read_register(EPROM_REG)) & (1 << 4)) );

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

  write_register(RREG_BUFL, (u32)rx_descs);
  write_register(RREG_BUFH, 0);
  write_register(RREG_BUFSIZE, 32*16);
  write_register(RREG_HEAD, 0);
  write_register(RREG_TAIL, 31);
  write_register(RREG_CONFIG, RVAL_CONFIG);
  rx_cur = 0;

  write_register(TREG_BUFL, (u32)tx_descs);
  write_register(TREG_BUFH, 0);
  write_register(TREG_BUFSIZE, 8*16);
  write_register(TREG_HEAD, 0);
  write_register(TREG_TAIL, 0);
  write_register(TREG_CONFIG, TVAL_CONFIG);
  write_register(TREG_FLOW, TVAL_FLOW);
  tx_cur = 0;
}

void nic_enable_interrupt() {
  write_register(GREG_INT_MASK, GVAL_INT_MASK);
}

u8 *get_mac_address() {
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
  u32 interrupt_cause = read_register(GREG_INT_CAUSE);

  return interrupt_cause;
}

u8 *init_nic(pci_device *device) {
  base_address = device->base_address_0;
  enable_bus_mastering(device);

  u8 *mac = get_mac_address();

  rx_tx_init();

  return mac;
}

rx_desc *nic_read_packet() {
  rx_desc *ret = (rx_desc *)(rx_descs + 16*rx_cur);

  if (ret->status != 0) {
    write_register(RREG_TAIL, rx_cur);
    rx_cur = (rx_cur + 1) % 32;
  }

  return ret;
}

void nic_send_packet(ethernet_packet *p_data, u16 p_len) {
  tx_desc *current = (tx_desc *)(tx_descs + 16*tx_cur);
  current->addrl = (u64)p_data;
  current->length = p_len + 10;
  current->cmd = TSEND_CMD;
  current->status = 0;

  tx_cur = (tx_cur + 1) % 8;

  asm volatile("cli");

  write_register(TREG_TAIL, tx_cur);
  while (current->status == 0) {}

  asm volatile("sti");
}
