#ifndef PRINT_H
#define PRINT_H

#include "../cpu/types.h"

void print_char(char c, u8 color, u8 bg);
void set_char(char c, u8 cololr, u8 bg);
void print_str(char* c, u8 color, u8 bg);
void set_cursor(u8 row, u8 col);

#endif
