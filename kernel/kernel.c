#include "../cpu/types.h"
#include "../interrupts/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/video.h"
#include "../lib/print.h"
#include "../lib/string.h"
#include "shell.h"

static int keyidx = 0;
static char keybuffer[256];
static u8 foreground_color = 0x01;
static u8 background_color = 0x00;

static void user_input(char input);

void kmain() {
  isr_install();
  irq_install();

  init_keyboard(user_input);

  video_init();
  clear_screen(background_color);
  set_cursor(0, 0);

  print_str("ZOS is a NOT an os \n>", foreground_color, background_color);
  set_char('_', foreground_color, background_color);

  set_display_buffer();
  video_draw();
}

static void user_input(char input) {
  print_char(input, foreground_color, background_color);

  if (input == '\n') {
    void (*command_handler)(char*,u8*,u8*) = get_command(keybuffer);
    command_handler(keybuffer, &foreground_color, &background_color);

    keybuffer[0] = '\0';
    keyidx = 0;

    print_char('>', foreground_color, background_color);
  } else if (input == '\b') {
    keybuffer[keyidx] = '\0';
    if (keyidx > 0) keyidx -= 1;
  } else {
    keybuffer[keyidx] = input;
    keybuffer[keyidx+1] = '\0';
    keyidx += 1;
  }

  set_char('_', foreground_color, background_color);
  set_display_buffer();
  video_draw();
}
