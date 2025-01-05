#ifndef ETHERNET_H
#define ETHERNET_H

#include "network.h"

#pragma pack(push, 1)
typedef struct {
  u8 src_mac[6];
  u8 dest_mac[6];
  u16 eth_type;
} ethernet_packet;
#pragma pack(pop)

void ethernet_send_packet(network_info *, u8 dest[6], u16, void *, u16);
void ethernet_recieve_packet(ethernet_packet *, u16);

#endif
