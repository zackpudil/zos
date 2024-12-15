#ifndef VIDEO_H
#define VIDEO_H

#include "../cpu/types.h"

#define VIDEO_MEMORY 0xa0000;

#define COLS 320
#define ROWS 200
#define BUFSIZE 320*200


void video_init();
void video_draw();
void swap_buffer();

void clear_screen(u8 color);
void fill_rect(u8 row, u8 col, u8 size, u8 color);
void fill_rect_mask(u8 row, u8 col, u8 size, u8 color, u8 *mask);

void draw_rect(u8 row, u8 col, u8 *d, u8 size);

#endif
