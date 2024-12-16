#include "../cpu/types.h"
#include "../interrupts/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/video.h"
#include "../lib/print.h"
#include "../lib/string.h"

static int keyidx = 0;
static char keybuffer[256];
static u8 screen_color = 0x01;

static void user_input(char input);
static void do_command();
static char num_to_char(u8 i);
static u8 char_to_num(char c);

void kmain() {
  isr_install();
  irq_install();

  init_keyboard(user_input);

  video_init();
  clear_screen(0);
  set_cursor(0, 0);

  print_str("TYPE ANYTHING: \n>", screen_color);
  set_char('_', screen_color);

  set_display_buffer();
  video_draw();
}

static void user_input(char input) {
  print_char(input, screen_color);

  if (input == '\n') {
    do_command();
    keybuffer[0] = '\0';
    keyidx = 0;
  } else if (input == '\b') {
    keybuffer[keyidx] = '\0';
    keyidx -= 1;
  } else {
    keybuffer[keyidx] = input;
    keybuffer[keyidx+1] = '\0';
    keyidx += 1;
  }

  set_char('_', screen_color);
  set_display_buffer();
  video_draw();
}

static void do_command() {
  if (str_begins_with("ECHO", keybuffer, 4)) {
    char *s = keybuffer;
    s += 5;
    print_str(s, screen_color);
    print_char('\n', 0x00);
  } else if(str_begins_with("SET_COLOR", keybuffer, 8)) {
    u8 fh = char_to_num(keybuffer[10]);
    u8 fl = char_to_num(keybuffer[11]);
    u8 f = (fh << 4) + fl;
    screen_color = f;
  } else if(str_begins_with("SHOW_COLOR", keybuffer, 10)) {
    clear_screen(0);
    set_cursor(0, 0);
    print_str("256 COLOR PALLET: \n", 0x1f);
    u8 currentColor = 0;
    for(u8 i = 0; i < 16; i++) {
      print_char(num_to_char(i), 0x1f);
      print_str("X: ", 0x1f);
      for(int i = 0; i < 16; i++) {
        print_char(num_to_char(i), currentColor);
        currentColor++;
      }
      print_char('\n', 0x00);
    }
  } else if (str_begins_with("CLEAR", keybuffer, 5)) {
    clear_screen(0);
    set_cursor(0, 0);
  } else if (str_begins_with("EXIT", keybuffer, 4)) {
    print_str("EXITING...........\n", screen_color);
    set_display_buffer();
    video_draw();
    asm("hlt");
  } else {
    print_str("UNKNOWN COMMAND\n", screen_color);
  }
  print_char('>', screen_color);
}

static u8 char_to_num(char c) {
  switch(c) {
    case '0': return 0x0;
    case '1': return 0x1;
    case '2': return 0x2;
    case '3': return 0x3;
    case '4': return 0x4;
    case '5': return 0x5;
    case '6': return 0x6;
    case '7': return 0x7;
    case '8': return 0x8;
    case '9': return 0x9;
    case 'A': return 0xa;
    case 'B': return 0xb;
    case 'C': return 0xc;
    case 'D': return 0xd;
    case 'E': return 0xe;
    case 'F': return 0xf;
    default: return 0;
  }
}

static char num_to_char(u8 i) {
  switch(i) {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return 'A';
    case 11: return 'B';
    case 12: return 'C';
    case 13: return 'D';
    case 14: return 'E';
    case 15: return 'F';
    default: return 'X';
  }
}


