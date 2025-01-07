#include "ip.h"
#include "arp.h"
#include "ethernet.h"
#include "udp.h"
#include "icmp.h"
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

  u8 dest_mac[6];

  if (dest_ip[0] == 255 && dest_ip[1] == 255 && dest_ip[2] == 255 && dest_ip[3] == 255) {
    mset(dest_mac, 0xff, 6);
  } else {
    u8 *mac = arp_get_mach_address(net, dest_ip, (u8[6]){255,255,255,255,255,255});

    if (mac[0] == 0 && mac[1] == 0 && mac[2] == 0 && mac[3] == 0 && mac[4] == 0) {
      mac = arp_get_mach_address(net, net->gateway_addr, (u8[6]){255,255,255,255,255,255});
    }

    mcopy(mac, dest_mac, 6);
  }

  ethernet_send_packet(net, dest_mac, 0x0800, packet, sizeof(ip_packet) + size);
}

void ip_recieve_packet(
    ip_packet *packet,
    void *data, u16 size)
{
  if (packet->protocol == 0x11) {
    udp_recieve_packet(packet, data, size + sizeof(udp_packet));
  } else if (packet->protocol == 0x01) {
    icmp_recieve_packet(packet, data, size + sizeof(icmp_packet));
  }
}
