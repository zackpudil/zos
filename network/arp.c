#include "arp.h"
#include "ethernet.h"
#include "../lib/mem.h"
#include "../cpu/bits.h"

#include "../lib/string.h"
#include "../lib/print.h"

static arp_requested_destination_mac[6] = {0,0,0,0,0,0};
static last_arp_requested_is_finished = false;

void arp_send_packet(network_info *net, u8 ip[4], u8 mac[6]) {
  arp_packet *packet = (arp_packet *)malloc(sizeof(arp_packet), false, 0);

  packet->hardware_type = switch_endian_16(1);
  packet->protocol_type = switch_endian_16(0x0800);
  packet->hardware_len = 6;
  packet->protocol_len = 4;
  packet->operation = switch_endian_16(1);

  mcopy(net->mac, packet->sender_mac, 6);
  mcopy(net->ip_addr, packet->sender_ip, 4);
  mcopy(ip, packet->dest_ip, 4);

  ethernet_send_packet(net, mac, 0x0806, packet, sizeof(arp_packet));
}

void arp_recieve_packet(arp_packet *packet) {
  mcopy(packet->sender_mac, arp_requested_destination_mac, 6);
  last_arp_requested_is_finished = true;
}

u8 *arp_get_mach_address(network_info *net, u8 dest_ip[4], u8 dest_mac[6]) {
  last_arp_requested_is_finished = false;
  arp_send_packet(net, dest_ip, dest_mac);

  while (!last_arp_requested_is_finished) {}

  return arp_requested_destination_mac;
} 

