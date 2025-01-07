#ifndef ARP_H
#define ARP_H

#include "../cpu/types.h"
#include "network.h"

#pragma pack(push, 1)
typedef struct {
  u16 hardware_type;
  u16 protocol_type;
  u8 hardware_len;
  u8 protocol_len;
  u16 operation;
  u8 sender_mac[6];
  u8 sender_ip[4];
  u8 dest_mac[6];
  u8 dest_ip[4];
} arp_packet;
#pragma pack(pop)

u8 *arp_get_mach_address(network_info *net, u8 dip[4], u8 dmac[6]);
void arp_recieve_packet(arp_packet *);

#endif
