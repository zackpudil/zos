#ifndef STRING_H
#define STRING_H

#include "../cpu/types.h"

bool str_begins_with(char *starts, char *test, u8 start_len);

char *byte_to_str(u8 c);
char *word_to_str(u16 c);
char *dword_to_str(u32 c);
char *number_to_string(u8 number);

u8 str_to_byte(char *s);
u16 str_to_word(char *s);
u32 str_to_dword(char *s);

u32 str_len(char *s);

u8 char_to_nibble(char c);
char nibble_to_char(u8 i);

#endif
