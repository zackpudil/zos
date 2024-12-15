#include "../cpu/types.h"
#include "../interrupts/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/video.h"
#include "ascii.h"

static u8 screen = 0x01;
static u8 row = 0;
static u8 col = 0;

static void user_input(char input);

void print_char(char c, u8 color);
void print_str(char *str);

void kmain() {
  isr_install();
  irq_install();

  init_keyboard(user_input);

  video_init();
  clear_screen(0);

  print_str("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-=;'\\,./!@#$%^&*()_+:\"|<>?\0");
  row = 1;
  col = 0;

  swap_buffer();
  video_draw();
}

static void user_input(char input) {
  if (input == '\n') {
    row += 1;
    col = 0;
    return;
  }

  if (input == '\b') {
    col -= 1;
    if (col < 0) {
      col = 0;
      row -= 1;
    }
    return;
  }

  print_char(input, screen);
  screen++;
  if (screen >= 0x10) {
    screen = 1;
  }
  
  swap_buffer();
  video_draw();
}

void print_char(char c, u8 color) {
  u8 *mask = get_mask(c);
  fill_rect_mask(row, col, 8, color, mask);

  col += 1;
  if (col >= 40) {
    col = 0;
    row += 1;
  }
}

void print_str(char *str) {
  u32 i = 0;
  while (str[i] != '\0') {
    print_char(str[i], screen);
    screen++;
    if (screen >= 0x10) {
      screen = 1;
    }
    i++;
  }
  swap_buffer();
  video_draw();
}
