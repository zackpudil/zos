#include "video.h"
#include "../lib/mem.h"

u8 *screen = (u8 *)VIDEO_MEMORY;

u8 *display_buffer;
u8 *working_buffer;

void video_init() {
  display_buffer = (u8 *)malloc(BUFSIZE, true, 0);
  working_buffer = (u8 *)malloc(BUFSIZE, true, 0);
}

void video_draw() {
  mcopy(display_buffer, screen, BUFSIZE);
}

void set_display_buffer() {
  mcopy(working_buffer, display_buffer, BUFSIZE);
}

void swap_buffers() {
  u8 *temp = display_buffer;
  display_buffer = working_buffer;
  working_buffer = temp;
}

void clear_screen(u8 color) {
  mset(working_buffer, color, BUFSIZE);
}

void draw_rect(u8 row, u8 col, u8 *d, u8 size) {
  u32 start_row = row*size;
  u32 start_col = col*size;

  for(u32 i = 0; i < size; i++) {
    for(u32 j = 0; j < size; j++) {
      u32 r = start_row + i;
      u32 c = start_col + j;

      working_buffer[c + r*COLS] = d[j + i*size];
    }
  }
}

void fill_rect(u8 row, u8 col, u8 size, u8 color) {
  u32 start_row = row*size;
  u32 start_col = col*size;

  for (u32 i = 0; i < size; i++) {
    for (u32 j = 0; j < size; j++) {
      u32 r = start_row + i;
      u32 c = start_col + j;

      working_buffer[c + r*COLS] = color;
    }
  }
}

void fill_rect_mask(u8 row, u8 col, u8 size, u8 color, u8 *mask)  {
  u32 start_row = row*size;
  u32 start_col = col*size;

  for(u32 i = 0; i < size; i++) {
    for(u32 j = 0; j < size; j++) {
      u32 r = start_row + i;
      u32 c = start_col + j;

      u8 row_mask = mask[i];
      bool col_mask = (row_mask >> (size-j)) & 1;

      working_buffer[c + r*COLS] = col_mask ? color : 0x00;
    }
  }
}
