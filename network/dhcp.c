#include "dhcp.h"
#include "udp.h"
#include "../lib/mem.h"
#include "../cpu/bits.h"

dhcp_packet *construct_packet() {
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
  mset(packet->chaddr, 0, 16);
  mset(packet->sname, 0, 64);
  mset(packet->file, 0, 128);

  return packet;
}

void dhcp_send_discover(network_info *net) {
  dhcp_packet *packet = construct_packet();
  mcopy(net->mac, packet->chaddr, 6);

  packet->options[0] = 0x35;
  packet->options[1] = 0x01;
  packet->options[2] = 0x01;
  packet->options[3] = 0xff;

  udp_send_packet(net,
      (u8[]){0,0,0,0}, 68,
      (u8[]){255,255,255,255}, 67,
      packet, sizeof(dhcp_packet) 
  );
}

void dhcp_request_offer(network_info *net, dhcp_packet *offer) {
  dhcp_packet *packet = construct_packet();
  mcopy(net->mac, packet->chaddr, 6);
  mcopy(offer->siaddr, packet->siaddr, 4);

  packet->options[0] = 0x35;
  packet->options[1] = 0x01;
  packet->options[2] = 0x03;
  
  packet->options[3] = 0x32;
  packet->options[4] = 0x04;
  packet->options[5] = offer->yiaddr[0];
  packet->options[6] = offer->yiaddr[1];
  packet->options[7] = offer->yiaddr[2];
  packet->options[8] = offer->yiaddr[3];

  packet->options[9] = 0x36;
  packet->options[10] = 0x04;
  packet->options[11] = offer->siaddr[0];
  packet->options[12] = offer->siaddr[1];
  packet->options[13] = offer->siaddr[2];
  packet->options[14] = offer->siaddr[3];
  packet->options[15] = 0xff;

  udp_send_packet(net,
      (u8[]){0,0,0,0}, 68,
      (u8[]){255,255,255,255}, 67,
      packet, sizeof(dhcp_packet)
  );
}
