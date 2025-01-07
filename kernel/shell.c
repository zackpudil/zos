#include "shell.h"
#include "../lib/print.h"
#include "../drivers/video.h"
#include "../lib/string.h"
#include "../network/network.h"

void print_color_pallet(char *keybuffer, u8 *f, u8 *b) {
  UNUSED(keybuffer);

  clear_screen(*b);
  set_cursor(0, 0);

  print_str("256 Color Pallet: \n", *f, *b);
  u8 currentColor = 0;
  for(u8 i = 0; i < 16; i++) {
    print_char(nibble_to_char(i), *f, *b);
    print_str("X: ", *f, *b);
    for(int i = 0; i < 16; i++) {
      print_char(nibble_to_char(i), currentColor, *b);
      currentColor++;
    }
    print_char('\n', 0x00, *b);
  }
}

void echo(char *keybuffer, u8 *f, u8 *b) {
  char *s = keybuffer+5;
  print_str(s, *f, *b);
  print_char('\n', 0x00, *b);
}

void clear(char *keybuffer, u8 *f, u8 *b) {
  UNUSED(keybuffer);
  UNUSED(f);

  clear_screen(*b);
  set_cursor(0, 0);
}

void exit(char *keybuffer, u8 *f, u8 *b) {
  UNUSED(keybuffer);

  print_str("Shutting Down...........\n", *f, *b);
  set_display_buffer();
  video_draw();
  asm("hlt");
}

void get_color(char *keybuffer, u8 *f, u8 *b) {
  UNUSED(keybuffer);

  print_char(nibble_to_char((*f >> 4) & 0x0F), *f, *b);
  print_char(nibble_to_char(*f & 0x0F), *f, *b);
  print_char('\n', 0x00, *b);
}

void set_color(char *keybuffer, u8 *f, u8 *b) {
  UNUSED(b);

  u8 nf = str_to_byte(keybuffer+10);
  *f = nf;
}

void get_bgcolor(char *keybuffer, u8 *f, u8 *b) {
  UNUSED(keybuffer);

  print_char(nibble_to_char((*b >> 4) & 0x0F), *f, *b);
  print_char(nibble_to_char(*b & 0x0F), *f, *b);
  print_char('\n', 0x00, *b);
}

void set_bgcolor(char *keybuffer, u8 *f, u8 *b) {
  UNUSED(f);

  u8 orig_background_color = *b;
  u8 nb = str_to_byte(keybuffer+12);
  *b = nb;
  remask(orig_background_color, *b);
}

void help(char *keybuffer, u8 *f, u8 *b) {
  UNUSED(keybuffer);

  print_str("show pallet: Shows all coloss\nget ip XXXXX: get ip address for domain", *f, *b); 
  print_str("set color XX: Set the color\nset bgcolor XX: Set the bg color\n", *f, *b);
  print_str("clear: Clear the sceeen\nexit: Stop computer\necho: Say something", *f, *b);
  print_char('\n', 0x00, *b);
}

void unknown_command(char *keybuffer, u8 *f, u8 *b) {
  print_str("Unknown Command\n", *f, *b);
}

void test_chars(char *keybuffer, u8 *f, u8 *b) {
  print_str("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", *f, *b);
  print_str("abcdefghijklmnopqrstuvwxyz\n", *f, *b);
  print_str("0123456789\n", *f, *b);
  print_str("`~!@#$%^&*()-=_+[]{};'\\:\"|,./<>?\n", *f, *b);
};

void get_ip(char *keybuffer, u8 *f, u8 *b) {
  print_char('\n', *f, *b);
  print_str("Getting ip address for: ", *f, *b);
  print_str(keybuffer + 7, *f, *b);
  print_char('\n', *f, *b);

  *(keybuffer + 6) = '.';
  u8 *ip = get_ip_addr(keybuffer + 6);

  print_str("Found IP Address: ", *f, *b);

  for(u8 i = 0; i < 4; i++) {
    print_str(number_to_string(ip[i]), *f, *b);
    if (i != 3) print_char('.', *f, *b);
  }

  print_char('\n', *f, *b);
}

void get_ping(char *keybuffer, u8 *f, u8 *b) {
  *(keybuffer + 4) = '.';
  print_char('\n', *f, *b);
  print_str("Pinging: ", *f, *b);
  print_str(keybuffer + 5, *f, *b);
  print_str("............\n", *f, *b);
  set_display_buffer();
  video_draw();

  u32 i = ping(keybuffer + 4);
  print_str("Ping done in (", *f, *b);
  print_str(dword_to_str(i), *f, *b);
  print_str(") cycles\n", *f, *b);
}

void (*get_command(char *keybuffer))(char *, u8 *, u8 *) {
  if (str_begins_with("echo", keybuffer, 4)) return echo;
  else if(str_begins_with("set color", keybuffer, 8)) return set_color;
  else if(str_begins_with("set bgcolor", keybuffer, 10)) return set_bgcolor;
  else if (str_begins_with("show color", keybuffer, 10)) return get_color;
  else if (str_begins_with("show bgcolor", keybuffer, 12)) return get_bgcolor;
  else if(str_begins_with("show pallet", keybuffer, 10)) return print_color_pallet;
  else if (str_begins_with("clear", keybuffer, 5)) return clear;
  else if (str_begins_with("exit", keybuffer, 4)) return exit;
  else if (str_begins_with("help", keybuffer, 4)) return help;
  else if (str_begins_with("test", keybuffer, 4)) return test_chars;
  else if (str_begins_with("get ip", keybuffer, 6)) return get_ip;
  else if (str_begins_with("ping", keybuffer, 4)) return get_ping;

  return unknown_command;
}

