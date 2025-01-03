#include "video.h"
#include "../lib/mem.h"

u8 *screen = (u8 *)VIDEO_MEMORY;

//u8 *display_buffer;
u8 *working_buffer;

void video_init() {
  //display_buffer = (u8 *)malloc(BUFSIZE, true, 0);
  working_buffer = (u8 *)malloc(BUFSIZE, true, 0);
}

void video_draw() {
  mcopy(working_buffer, screen, BUFSIZE);
}

void set_display_buffer() {
  //mcopy(working_buffer, display_buffer, BUFSIZE);
}

void swap_buffers() {
  //u8 *temp = display_buffer;
  //display_buffer = working_buffer;
  //working_buffer = temp;
}

void clear_screen(u8 color) {
  mset(working_buffer, color, BUFSIZE);
}

void fill_rect_mask(u8 row, u8 col, u8 size, u8 on_color, u8 off_color, u8 *mask)  {
  u32 start_row = row*size;
  u32 start_col = col*size;

  for(u32 i = 0; i < size; i++) {
    for(u32 j = 0; j < size; j++) {
      u32 r = start_row + i;
      u32 c = start_col + j;

      u8 row_mask = mask[i];
      bool col_mask = (row_mask >> (size-j)) & 1;

      working_buffer[c + r*COLS] = col_mask ? on_color : off_color;
    }
  }
}

void remask(u8 orig_col, u8 new_col) {
  for(u32 i = 0; i < ROWS; i++)
  for(u32 j = 0; j < COLS; j++) {
    if (working_buffer[j + i*COLS] == orig_col)
      working_buffer[j + i*COLS] = new_col;
  }
}
