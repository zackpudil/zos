#include "network.h"
#include "ethernet.h"
#include "udp.h"
#include "dhcp.h"
#include "arp.h"
#include "dns.h"

#include "../interrupts/isr.h"
#include "../drivers/nic.h"
#include "../lib/mem.h"
#include "../cpu/pci.h"

#include "../lib/string.h"
#include "../lib/print.h"

static network_info *net_info;
static bool network_offer_receieved = false;
static bool network_requst_accepted = false;

static void network_interrupt(registers_t regs) {
  u32 status = nic_handle_interrupt();

  if (status & 0x80 || status & 0x10) {
    rx_desc *rx = nic_read_packet();

    while (rx->status & 0x1) {

      ethernet_packet *packet = (ethernet_packet*)rx->addrl;
      ethernet_recieve_packet(packet, rx->length);

      rx->status = 0;
      rx = nic_read_packet();
    }
  }
}

static void handle_dhcp_packet(void *data, u16 size) {
  dhcp_packet *packet = (dhcp_packet *)data;

  if (!network_offer_receieved) {
    dhcp_request_offer(net_info, packet);
    network_offer_receieved = true;
  } else if (!network_requst_accepted) {
    mcopy(packet->options + 11, net_info->subnet_mask, 4);
    mcopy(packet->options + 17, net_info->gateway_addr, 4);
    mcopy(packet->options + 23, net_info->dns_server, 4);
    mcopy(packet->yiaddr, net_info->ip_addr, 4);

    network_requst_accepted = true;
  }
}

network_info *init_network(pci_device *device) {
  udp_init();
  net_info = (network_info *)malloc(sizeof(network_info), false, 0);
  u8 *mac = init_nic(device);

  mcopy(mac, net_info->mac, 6);

  register_interrupt_handler(IRQ0 + device->int_line, network_interrupt);
  nic_enable_interrupt();

  udp_install_instener(68, handle_dhcp_packet);

  dhcp_send_discover(net_info);

  while (!network_requst_accepted) { }

  dns_get_answers(net_info, ".www.google.com");


  return net_info;
}

