#include "ip.h"
#include "ethernet.h"
#include "udp.h"
#include "../lib/mem.h"
#include "../cpu/bits.h"

u32 ip_calculate_checksum(ip_packet *packet) {
  u32 checksum = 0;

  u16 *data = (u16 *)packet;

  for(u32 i = 0; i < sizeof(ip_packet)/2; i++)
    checksum += data[i];

  while (checksum >> 16) 
    checksum = (checksum & 0xFFFF) + (checksum >> 16);

  return ~checksum;
}

void ip_send_packet(
     network_info *net,
     u8 source_ip[4],
     u8 dest_ip[4],
     u8 prot,
     void *data,
     u16 size)
{

  ip_packet *packet = (ip_packet *)malloc(sizeof(ip_packet) + size, false, 0);
  packet->version = 4;
  packet->ihl = 5;
  packet->dscp = 0;
  packet->ecn = 0;
  packet->total_length = switch_endian_16(sizeof(ip_packet) + size);
  packet->identification = 0;
  packet->flags = 0;
  packet->fragment_offset = 0;
  packet->ttl = 64;
  packet->protocol = prot;
  packet->header_checksum = 0;
  mcopy(source_ip, packet->source_ip, 4);
  mcopy(dest_ip, packet->destination_ip, 4);

  mcopy(data, (u8 *)packet + sizeof(ip_packet), size);

  packet->header_checksum = ip_calculate_checksum(packet);

  // TODO: implemnet ARP, currently just doing DHCP
  u8 dest_mac[6];
  mset(dest_mac, 0xff, 6);

  ethernet_send_packet(net, dest_mac, 0x0800, packet, sizeof(ip_packet) + size);
}

void ip_recieve_packet(
    ip_packet *packet,
    void *data, u16 size)
{
  if (packet->protocol == 0x11) {
    udp_recieve_packet(packet, data, size + sizeof(udp_packet));
  }
}
