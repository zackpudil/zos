#include "network.h"
#include "ethernet.h"
#include "udp.h"
#include "dhcp.h"

#include "../interrupts/isr.h"
#include "../drivers/nic.h"
#include "../lib/mem.h"
#include "../cpu/pci.h"

#include "../lib/print.h"
#include "../lib/string.h"


static network_info *net_info;

static void network_interrupt(registers_t regs) {
  u32 status = nic_handle_interrupt();

  print_str(dword_to_str(status), 0x01, 0x00);
  print_char('\n', 0x00, 0x00);

  if (status == 0x16 || status == 0x80) {
    rx_desc *rx = nic_read_packet();

    ethernet_packet *packet = (ethernet_packet*)rx->addrh;

    ethernet_recieve_packet(packet, rx->length);
  }
}

static void handle_upd_packet(void *data, u16 size) {
  dhcp_packet *packet = (dhcp_packet *)data;

  for(u8 i = 0; i < 4; i++) {
    print_str(byte_to_str(packet->yiaddr[i]), 0x01, 0x00);
    print_char('.', 0x01, 0x00);
  }
}

network_info *init_network(pci_device *device) {
  udp_init();
  net_info = (network_info *)malloc(sizeof(network_info), false, 0);
  u8 *mac = init_nic(device);

  mcopy(mac, net_info->mac, 6);

  register_interrupt_handler(IRQ0 + device->int_line, network_interrupt);
  nic_enable_interrupt();

  udp_install_instener(68, handle_upd_packet);

  dhcp_send_discover(net_info);

  return net_info;
}

