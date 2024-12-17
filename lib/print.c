#include "print.h"
#include "ascii.h"
#include "../drivers/video.h"

static u8 row = 0;
static u8 col = 0;

void set_char(char c, u8 color, u8 bg) {
  u8 *mask = get_mask(c);
  fill_rect_mask(row, col, 8, color, bg, mask);
}

void print_char(char c, u8 color, u8 bg) {
  bool inc = true;

  u8 *mask = get_mask(' ');
  fill_rect_mask(row, col, 8, color, bg, mask);

  if (c == '\n') {
    row += 1;
    col = 0;
    inc = false;
  }

  if (c == '\b') {
    if (col > 1) {
      col -= 1;
    }
    inc = false;
  }

  if (inc) {
    mask = get_mask(c);
    fill_rect_mask(row, col, 8, color, bg, mask);
    col += 1;
    if (col >= 40) {
      col = 0;
      row += 1;
    }
  }
}

void print_str(char *str, u8 color, u8 bg) {
  u32 i = 0;
  while (str[i] != '\0') {
    print_char(str[i], color, bg);
    i++;
  }
}

void set_cursor(u8 r, u8 c) {
  row = r;
  col = c;
}
