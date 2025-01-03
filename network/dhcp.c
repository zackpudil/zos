#include "dhcp.h"
#include "udp.h"
#include "../lib/mem.h"
#include "../cpu/bits.h"

void dhcp_send_discover(network_info *net) {
  dhcp_packet *packet = (dhcp_packet *)malloc(sizeof(dhcp_packet), false, 0);
  packet->op = 0x01;
  packet->htype = 0x01;
  packet->hlen = 6;
  packet->hops = 0;
  packet->magic_cookie = switch_endian_32(0x63825363);
  packet->xid = switch_endian_32(0x12345678);
  packet->secs = 0;
  packet->flags = 0;

  mset(packet->ciaddr, 0, 4);
  mset(packet->yiaddr, 0, 4);
  mset(packet->siaddr, 0, 4);
  mset(packet->giaddr, 0, 4);
  mcopy(net->mac, packet->chaddr, 6);
  mset(packet->chaddr + 6, 0, 10);
  mset(packet->sname, 0, 64);
  mset(packet->file, 0, 128);

  packet->options[0] = 0x35;
  packet->options[1] = 1;
  packet->options[2] = 0x01;
  packet->options[3] = 0xff;

  udp_send_packet(net,
      (u8[]){0, 0, 0, 0}, 68,
      (u8[]){255, 255, 255, 255}, 67,
      packet, sizeof(dhcp_packet)
  );
}
