#ifndef PRINT_H
#define PRINT_H

#include "../cpu/types.h"

void print_char(char c, u8 color);
void set_char(char c, u8 cololr);
void print_str(char* c, u8 color);
void set_cursor(u8 row, u8 col);

#endif
