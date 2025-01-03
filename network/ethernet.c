#include "network.h"
#include "ethernet.h"
#include "../drivers/nic.h"
#include "ip.h"
#include "../lib/mem.h"
#include "../cpu/bits.h"

void ethernet_send_packet(network_info *net, u8 dest[6], u16 type, void *data, u16 size) {
  ethernet_packet *eth_packet = (ethernet_packet *)malloc(sizeof(ethernet_packet) + size, true, 0);
  mcopy(net->mac, eth_packet->src_mac, 6);
  mcopy(dest, eth_packet->dest_mac, 6);

  eth_packet->eth_type = switch_endian_16(type);
  mcopy(data, (u8 *)eth_packet + sizeof(ethernet_packet), size);

  nic_send_packet(eth_packet, size + sizeof(eth_packet));
}

void ethernet_recieve_packet(ethernet_packet *eth, u16 size) {
  u16 type = switch_endian_16(eth->eth_type);

  if (type == 0x0800) {
    ip_recieve_packet(
        (ip_packet *)((u8 *)eth + sizeof(ethernet_packet)),
        (u8 *)eth + sizeof(ethernet_packet) + sizeof(ip_packet),
        size - sizeof(ip_packet)
    );
  }
}
