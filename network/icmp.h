#ifndef ICMP_H
#define ICMP_H

#include "../cpu/types.h"
#include "ip.h"
#include "network.h"

#pragma pack(push, 1)
typedef struct {
  u8 type;
  u8 code;
  u16 checksum;
  u8 data[4];
} icmp_packet;
#pragma pack(pop)

u32 icmp_ping(network_info *, u8 dest[4]);

void icmp_send_packet(network_info *, u8 dest[4]);
void icmp_recieve_packet(ip_packet *packet, void *data, u16 size);

#endif
