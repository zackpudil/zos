#ifndef NIC_H
#define NIC_H

#include "../network/ethernet.h"
#include "../cpu/types.h"
#include "../cpu/pci.h"

#pragma pack(push, 1)
typedef struct {
  volatile u32 addrl;
  volatile u32 addrh;
  volatile u16 length;
  volatile u16 checksum;
  volatile u8 status;
  volatile u8 errors;
  volatile u16 special;
} rx_desc;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
  volatile u32 addrl;
  volatile u32 addrh;
  volatile u16 length;
  volatile u8 cso;
  volatile u8 cmd;
  volatile u8 status;
  volatile u8 css;
  volatile u8 rsv;
  volatile u16 special;
} tx_desc;
#pragma pack(pop)

u32 nic_handle_interrupt();
void nic_enable_interrupt();

rx_desc *nic_read_packet();
void nic_send_packet(ethernet_packet *, u16);

u8 *init_nic(pci_device *device);

#endif
