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

#define EPROM_REG      0x0014
#define RREG_BUFL      0x2800
#define RREG_BUFH      0x2804
#define RREG_BUFSIZE   0x2808 
#define RREG_HEAD      0x2810
#define RREG_TAIL      0x2818
#define RREG_CONFIG    0x0100
#define RVAL_CONFIG    0x202803E

#define TREG_BUFL      0x3800
#define TREG_BUFH      0x3804
#define TREG_BUFSIZE   0x3808 
#define TREG_HEAD      0x3810
#define TREG_TAIL      0x3818
#define TREG_CONFIG    0x0400
#define TVAL_CONFIG    0x10400FA
#define TREG_FLOW      0x0410
#define TVAL_FLOW      0xA0280A
#define TSEND_CMD      0x0D

#define GREG_INT_MASK  0x00D0
#define GVAL_INT_MASK  0x1F6DC
#define GREG_INT_CAUSE 0x00C0

u32 nic_handle_interrupt();
void nic_enable_interrupt();

rx_desc *nic_read_packet();
void nic_send_packet(ethernet_packet *, u16);

u8 *init_nic(pci_device *device);

#endif
