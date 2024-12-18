#ifndef STRING_H
#define STRING_H

#include "../cpu/types.h"

bool str_begins_with(char *starts, char *test, u8 start_len);
u8 char_to_num(char c);
char num_to_char(u8 i);

#endif
