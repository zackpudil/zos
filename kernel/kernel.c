#include "../cpu/types.h"
#include "../cpu/pci.h"
#include "../interrupts/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/video.h"
#include "../drivers/nic.h"
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

  kprint_str("ZOS is a NOT an os \n");
  kprint_str("Found devices: \n");
  for(u8 i = 0; i < 4; i++) {
    pci_device *device = devices[i];

    if(!device->vendor_id) break;
    kprint_str("  B:");
    kprint_char(nibble_to_char(device->bus));
    kprint_str(" S:");
    kprint_char(nibble_to_char(device->slot));

    kprint_str(" V:");
    kprint_str(word_to_str(device->vendor_id));

    kprint_str(" D:");
    kprint_str(word_to_str(device->device_id));

    kprint_str(" C:");
    kprint_str(byte_to_str(device->class_code));
    kprint_str(" S:");
    kprint_str(byte_to_str(device->sub_class));
    kprint_str(" H:");
    kprint_str(byte_to_str(device->header_type));
    kprint_char('\n');

    if (device->base_address_0) {
      kprint_str("   BA 0:");
      kprint_str(dword_to_str(device->base_address_0));
      kprint_char('\n');
    }

    if (device->base_address_1) {
      kprint_str("   BA 1:");
      kprint_str(dword_to_str(device->base_address_1));
      kprint_char('\n');
    }

    if(device->base_rom_address) {
      kprint_str("   BA R:");
      kprint_str(dword_to_str(device->base_rom_address));
      kprint_char('\n');
    }

    kprint_str("   IP:");
    kprint_str(byte_to_str(device->int_pin));
    kprint_str(" IL:");
    kprint_str(byte_to_str(device->int_line));
    kprint_char('\n');
  }

  u8 *mac = init_network(devices[3]);

  set_display_buffer();
  video_draw();

  kprint_str("Found MAC address: \n  ");
  for(u8 i = 0; i < 6; i++) {
    char *word = byte_to_str(mac[i]);
    kprint_str(word);
    if (i != 5) kprint_str("::");
  }
  kprint_char('\n');

  kprint_char('>');
  set_char('_', foreground_color, background_color);

  set_display_buffer();
  video_draw();
  const char *data = "\xff\xff\xff\xff\xff\xff\x48\xb4\x23\xd5\xcf\x52\x08\x06\x00\x01\x08\x00\x06\x04\x00\x01\x48\xb4\x23\xd5\xcf\x52\xc0\xa8\x00\x0f\x00\x00\x00\x00\x00\x00\xc0\xa8\x00\x08\xea\xf8\x42\x15\x30\x99\xaa\xd2\xb1\x60\x75\x2f\x00\x00\x5e\x3e\xd9\x5d\xca\xd4\x14\xd4";

  send_packet(data, 64);
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
