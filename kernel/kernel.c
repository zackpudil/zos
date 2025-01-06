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
  irq_install();

  init_keyboard(user_input);

  video_init();
  pci_device **devices = get_all_devices();

  clear_screen(background_color);
  set_cursor(0, 0);

  print_str("ZOS is a NOT an os \n", 0x2f, 0x3f);
  kprint_char('\n');

  set_display_buffer();
  video_draw();

  kprint_str("Found Network Device: \n  VendorId: ");
  kprint_str(word_to_str(devices[3]->vendor_id));
  kprint_str("  DeviceId: ");
  kprint_str(word_to_str(devices[3]->device_id));
  kprint_str("\n\n");

  print_str("Loading Network Info...\n", 0x28, 0);

  set_display_buffer();
  video_draw();

  set_cursor(5, 0);

  network_info *nic = init_network(devices[3]);

  
  kprint_str("Network Info:                     \n");

  kprint_str("  IP Address: ");
  for(u8 i = 0; i < 4; i++) {
    char *word = number_to_string(nic->ip_addr[i]);
    kprint_str(word);
    if (i != 3) kprint_str(".");
  }

  kprint_str("\n  MAC address: ");
  for(u8 i = 0; i < 6; i++) {
    char *word = byte_to_str(nic->mac[i]);
    kprint_str(word);
    if (i != 5) kprint_str(":");
  }

  kprint_str("\n  Gateway Address: ");
  for(u8 i = 0; i < 4; i++) {
    char *word = number_to_string(nic->gateway_addr[i]);
    kprint_str(word);
    if (i != 3) kprint_str(".");
  }

  kprint_str("\n  Gateway MAC: ");
  for(u8 i = 0; i < 6; i++) {
    char *word = byte_to_str(nic->gateway_mac[i]);
    kprint_str(word);
    if (i != 5) kprint_str(":");
  }

  kprint_str("\n  Subnet Mask: ");
  for(u8 i = 0; i < 4; i++) {
    char *word = number_to_string(nic->subnet_mask[i]);
    kprint_str(word);
    if (i != 3) kprint_str(".");
  }

  kprint_str("\n  DNS Server: ");
  for(u8 i = 0; i < 4; i++) {
    char *word = number_to_string(nic->dns_server[i]);
    kprint_str(word);
    if (i != 3) kprint_str(".");
  }

  kprint_str("\n  DNS MAC: ");
  for(u8 i = 0; i < 6; i++) {
    char *word = byte_to_str(nic->dns_mac[i]);
    kprint_str(word);
    if (i != 5) kprint_str(":");
  }

  kprint_char('\n');

  kprint_char('>');
  set_char('_', foreground_color, background_color);

  set_display_buffer();
  video_draw();
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
  set_display_buffer();
  video_draw();
}
