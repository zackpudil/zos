#include "print.h"
#include "../kernel/ascii.h"
#include "../drivers/video.h"

static u8 row = 0;
static u8 col = 0;

void print_char(char c, u8 color) {
  bool inc = true;

  u8 *mask = get_mask(' ');
  fill_rect_mask(row, col, 8, color, mask);

  if (c == '\n') {
    row += 1;
    col = 0;
    inc = false;
  }

  if (c == '\b') {
    if (col == 0) {
      row -= 1;
      col = 39; 
    } else {
      col -= 1;
    }
    inc = false;
  }

  if (inc) {
    mask = get_mask(c);
    fill_rect_mask(row, col, 8, color, mask);
    col += 1;
    if (col >= 40) {
      col = 0;
      row += 1;
    }
  }

  mask = get_mask('_');
  fill_rect_mask(row, col, 8, color, mask);
}

void print_str(char *str, u8 color) {
  u32 i = 0;
  while (str[i] != '\0') {
    print_char(str[i], color);
    i++;
  }
  swap_buffer();
  video_draw();
}
