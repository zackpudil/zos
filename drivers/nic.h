#ifndef NETWORK_H
#define NETWORK_H

#include "../cpu/types.h"
#include "../cpu/pci.h"

typedef struct {
  volatile u64 addr;
  volatile u16 length;
  volatile u16 checksum;
  volatile u8 status;
  volatile u8 errors;
  volatile u16 special;
} rx_desc;

typedef struct {
  volatile u64 addr;
  volatile u16 length;
  volatile u8 cso;
  volatile u8 cmd;
  volatile u8 status;
  volatile u8 css;
  volatile u16 special;
} tx_desc;

u8 *init_network(pci_device *device);
void send_packet(void *p_data, u16 p_len);

#endif
