#ifndef DHCP_H
#define DHCP_H

#include "../cpu/types.h"
#include "network.h"

#pragma pack(push, 1)
typedef struct {
  u8 op;
  u8 htype;
  u8 hlen;
  u8 hops;
  u32 xid;
  u16 secs;
  u16 flags;
  u8 ciaddr[4];
  u8 yiaddr[4];
  u8 siaddr[4];
  u8 giaddr[4];
  u8 chaddr[16];
  u8 sname[64];
  u8 file[128];
  u32 magic_cookie;
  u8 options[312];
} dhcp_packet;
#pragma pack(pop)

void dhcp_send_discover(network_info *);
void dhcp_request_offer(network_info *, dhcp_packet *);

#endif
