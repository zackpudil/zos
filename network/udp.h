#ifndef UDP_H
#define UDP_H

#include "../cpu/types.h"
#include "network.h"
#include "ip.h"

#pragma pack(push, 1)
typedef struct {
  u16 source_port;
  u16 destination_port;
  u16 length;
  u16 checksum;
} udp_packet;
#pragma pack(pop)

typedef void (*udp_listener)(void *, u16);

void udp_init();

void udp_send_packet(network_info *, u8 s[4], u16, u8 d[4], u16, void *, u16);
void udp_recieve_packet(ip_packet *, void *, u16);

void udp_install_instener(u16, udp_listener);
void udp_remove_listner(u16);
#endif
