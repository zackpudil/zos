#ifndef SHELL_H
#define SHELL_H
#include "../cpu/types.h"

void print_color_pallet(char *keybuffer, u8 *f, u8 *b);
void echo(char *keybuffer, u8 *f, u8 *b);
void clear(char *keybuffer, u8 *f, u8 *b);
void exit(char *keybuffer, u8 *f, u8 *b);
void help(char *keybuffer, u8 *f, u8 *b);
void unknown_command(char *keybuffer, u8 *f, u8 *b);

void get_color(char *keybuffer, u8 *f, u8 *b);
void set_color(char *keybuffer, u8 *f, u8 *b);

void get_bgcolor(char *keybuffer, u8 *f, u8 *b);
void set_bgcolor(char *keybuffer, u8 *f, u8 *b);

void (*get_command(char *keybuffer))(char *, u8 *, u8 *);

#endif
