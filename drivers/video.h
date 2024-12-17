#ifndef VIDEO_H
#define VIDEO_H

#include "../cpu/types.h"

#define VIDEO_MEMORY 0xa0000;

#define COLS 320
#define ROWS 200
#define BUFSIZE 320*200


void video_init();
void video_draw();
void swap_buffers();
void set_display_buffer();

void clear_screen(u8 color);
void fill_rect(u8 row, u8 col, u8 size, u8 color);
void fill_rect_mask(u8 row, u8 col, u8 size, u8 on_color, u8 off_color, u8 *mask);

void remask(u8 orig_col, u8 new_col);
void draw_rect(u8 row, u8 col, u8 *d, u8 size);

#endif
