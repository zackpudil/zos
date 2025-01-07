#include "dns.h"
#include "udp.h"
#include "../lib/string.h"
#include "../lib/print.h"
#include "../cpu/bits.h"

static u8 dns_response[4];
static bool dns_packet_recieved = false;

static void dns_handle_recieve(void *data, u16 size) {
  mcopy(data + 44, dns_response, 4);
  dns_packet_recieved = true;
}

u8 *dns_get_answers(network_info *net, char *labels) {
  u16 packet_size = sizeof(dns_packet) + str_len(labels) + 5;

  dns_packet *packet = (dns_packet *)malloc(packet_size, false, 0);

  packet->transaction_id = switch_endian_16(0x1234567);
  packet->flags = 1 << 8;
  packet->questions = switch_endian_16(1);
  packet->answers = 0;
  packet->auth_rrs = 0;
  packet->additional_rrs = 0;

  u8 j = 0;
  for(int i = str_len(labels) - 1; i >= 0; i--) {
    if(labels[i] == '.') {
      labels[i] = j;
      j = 0;
    } else j++;
  }

  mcopy(labels, (u8 *)packet + sizeof(dns_packet), str_len(labels));
  mset((u8 *)packet + sizeof(dns_packet) + str_len(labels) + 1, 0, 1);
  mset((u8 *)packet + sizeof(dns_packet) + str_len(labels) + 2, 1, 1);
  mset((u8 *)packet + sizeof(dns_packet) + str_len(labels) + 3, 0, 1);
  mset((u8 *)packet + sizeof(dns_packet) + str_len(labels) + 4, 1, 1);
  mset((u8 *)packet + sizeof(dns_packet) + str_len(labels) + 5, 0, 1);

  udp_install_instener(54, dns_handle_recieve);
  udp_send_packet(net, net->ip_addr, 54, net->dns_server, 53, packet, packet_size); 

  while(!dns_packet_recieved) { }

  udp_remove_listner(54);

  return dns_response;
}
