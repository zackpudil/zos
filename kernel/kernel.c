#include "../cpu/types.h"
#include "../cpu/pci.h"
#include "../interrupts/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/video.h"
#include "../network/network.h"
#include "../lib/print.h"
#include "../lib/string.h"
#include "shell.h"

static int keyidx = 0;
static char keybuffer[256];
static u8 foreground_color = 0x01;
static u8 background_color = 0x00;

static void user_input(char input);

void kprint_str(char *s) { print_str(s, foreground_color, background_color); }
void kprint_char(char s) { print_char(s, foreground_color, background_color); }

void kmain() {
  isr_install();
  init_keyboard(user_input);
  video_init();

  clear_screen(background_color);
  set_cursor(0, 0);
  print_str("ZOS is a NOT an os \n", 0x2f, 0x3f);
  kprint_char('\n');
  video_draw();

  pci_device **devices = get_all_devices();

  kprint_str("Found Network Device: \n  VendorId: ");
  kprint_str(word_to_str(devices[3]->vendor_id));
  kprint_str("  DeviceId: ");
  kprint_str(word_to_str(devices[3]->device_id));
  kprint_str("\n\n");

  print_str("Loading Network Info...\n", 0x28, 0);

  video_draw();

  network_info *nic = init_network(devices[3]);
  
  kprint_str("Network Info: \n");

  print_net_data(nic->ip_addr, "  IP Address: ", 4, '.', number_to_string);
  print_net_data(nic->mac, "\n  MAC Address: ", 6, ':', byte_to_str);
  print_net_data(nic->gateway_addr, "\n  Gateway Address: ", 4, '.', number_to_string);
  print_net_data(nic->subnet_mask, "\n  Subnet Mask: ", 4, '.', number_to_string);
  print_net_data(nic->dns_server, "\n  DNS Server: ", 4, '.', number_to_string);

  kprint_char('\n'); 

  kprint_char('>');
  set_char('_', foreground_color, background_color);

  video_draw();
}

void print_net_data(u8 *data, char *header, int size, char seperatr, char *(*str_hnd)(u8)) {
  kprint_str(header);
  for(u8 i = 0; i < size; i++) {
    char *word = str_hnd(data[i]);
    kprint_str(word);
    if (i != size - 1) kprint_char(seperatr);
  }
}

static void user_input(char input) {
  print_char(input, foreground_color, background_color);

  if (input == '\n') {
    void (*command_handler)(char*,u8*,u8*) = get_command(keybuffer);
    command_handler(keybuffer, &foreground_color, &background_color);

    keybuffer[0] = '\0';
    keyidx = 0;

    print_char('>', foreground_color, background_color);
  } else if (input == '\b') {
    keybuffer[keyidx] = '\0';
    if (keyidx > 0) keyidx -= 1;
  } else {
    keybuffer[keyidx] = input;
    keybuffer[keyidx+1] = '\0';
    keyidx += 1;
  }

  set_char('_', foreground_color, background_color);
  video_draw();
}
