#ifndef SHELL_H
#define SHELL_H
#include "../cpu/types.h"

void (*get_command(char *keybuffer))(char *, u8 *, u8 *);

#endif
