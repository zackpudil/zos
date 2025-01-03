#ifndef IP_H
#define IP_H

#include "network.h"
#include "../cpu/types.h"

#pragma pack(push, 1)
typedef struct {
  u8 ihl: 4;
  u8 version: 4;
  u8 ecn: 2;
  u8 dscp: 6;
  u16 total_length;
  u16 identification;
  u16 fragment_offset: 13;
  u8 flags: 3;
  u8 ttl;
  u8 protocol;
  u16 header_checksum;
  u8 source_ip[4];
  u8 destination_ip[4];
} ip_packet;
#pragma pack(pop)

void ip_send_packet(network_info *, u8 sip[4], u8 dip[4], u8, void *, u16);
void ip_recieve_packet(ip_packet *, void *, u16);


#endif
