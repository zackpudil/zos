#include "udp.h"
#include "../lib/mem.h"
#include "../cpu/bits.h"

static udp_listener *udp_listeners;

void udp_init() {
  udp_listeners = (udp_listener *)malloc(sizeof(udp_listener)*100, false, 0);
}

void udp_send_packet(
    network_info *net,
    u8 source_ip[4], u16 source_port,
    u8 dest_ip[4], u16 dest_port,
    void *data, u16 size)
{
  udp_packet *packet = (udp_packet *)malloc(sizeof(udp_packet) + size, false, 0);
  mset((u8 *)packet, 0, sizeof(udp_packet) + size);

  packet->source_port = switch_endian_16(source_port);
  packet->destination_port = switch_endian_16(dest_port);

  packet->length = switch_endian_16(size + sizeof(udp_packet));
  packet->checksum = 0;

  mcopy(data, (u8 *)packet + sizeof(udp_packet), size);

  ip_send_packet(net, source_ip, dest_ip, 0x11, packet, sizeof(udp_packet) + size); 
}

void udp_recieve_packet(ip_packet *ip_packet, void *data, u16 size) {
  udp_packet *packet = (udp_packet *)data;

  u16 port = switch_endian_16(packet->destination_port);

  if (udp_listeners[port]) {
    udp_listeners[port](data + sizeof(udp_packet), size - sizeof(udp_packet));
  }
}


void udp_install_instener(u16 port, udp_listener listener) {
  udp_listeners[port] = listener;
}

void udp_remove_listner(u16 port) {
  udp_listeners[port] = 0;
}
