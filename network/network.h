#ifndef NETWORK_H 
#define NETWORK_H

#include "../cpu/types.h"

typedef struct {
  u8 mac[6];
  u8 ip_addr[4];
  u8 gateway_addr[4];
} network_info;

network_info *init_network();

#endif
