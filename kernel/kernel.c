#include "../cpu/types.h"
#include "../interrupts/isr.h"
#include "../drivers/keyboard.h"
#include "../lib/print.h"
#include "ascii.h"

static u8 screen = 0x01;
static void user_input(char input);

void kmain() {
  isr_install();
  irq_install();

  init_keyboard(user_input);

  video_init();
  clear_screen(0);

  print_str("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\n\0", 0x0a);
  print_str("`~!@#$%^&*()-=_+[]{};':\"|\\,./<>?\n\0", 0x0a);

  swap_buffer();
  video_draw();
}

static void user_input(char input) {
  print_char(input, screen);
  screen++;
  if (screen >= 0x10) {
    screen = 1;
  }
  
  swap_buffer();
  video_draw();
}

