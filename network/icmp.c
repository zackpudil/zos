#include "icmp.h"
#include "ip.h"
#include "../lib/mem.h"

static bool icmp_block = false;

u32 icmp_ping(network_info *net, u8 dest[4]) {
  u32 i = 0;
  icmp_block = true;
  icmp_send_packet(net, dest);
  while (icmp_block) i++;

  return i;
}

u32 icmp_calculate_checksum(icmp_packet *packet) {
  u32 checksum = 0;

  u16 *data = (u16 *)packet;
  for(u32 i = 0; i < sizeof(icmp_packet)/2; i++)
    checksum += data[i];

  while (checksum >> 16)
    checksum = (checksum & 0xFFFF) + (checksum >> 16);

  return ~checksum;
}

void icmp_send_packet(network_info *net, u8 dest[4]) {
  icmp_packet *packet = (icmp_packet *)malloc(sizeof(icmp_packet), false, 0);

  packet->type = 8;
  packet->code = 0;
  packet->checksum = 0;
  mset(packet->data, 0, 4);

  packet->checksum = icmp_calculate_checksum(packet);

  ip_send_packet(
       net, net->ip_addr,
       dest, 0x01, packet, sizeof(icmp_packet)
  );
}

void icmp_recieve_packet(ip_packet *packet, void *data, u16 size) {
  icmp_block = false;
  // do nothing for now.
}
