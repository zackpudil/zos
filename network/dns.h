#ifndef DNS_H
#define DNS_H

#include "../cpu/types.h"
#include "network.h"

#pragma pack(push, 1)
typedef struct {
  u16 transaction_id;
  u16 flags;
  u16 questions;
  u16 answers;
  u16 auth_rrs;
  u16 additional_rrs;
} dns_packet;
#pragma pack(pop)

char *dns_get_answers(network_info *, char *);

#endif
