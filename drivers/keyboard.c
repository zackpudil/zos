#include "keyboard.h"
#include "../interrupts/isr.h"
#include "../cpu/ports.h"
#include "../cpu/types.h"

const char sc_ascii_lower[] = {
  '\0',
  '\0',  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
  '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
  '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\0',
  '\0', '\0', ' '
};

const char sc_asccii_upper[] = {
  '\0',
  '\0',  '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
  '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
  '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
  '\0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '\0',
  '\0', '\0', ' '
};

static bool shift_down = false;

static void (*char_handler)(char) = 0x0;

static void keyboard_callback(registers_t regs) {
  u8 scancode = port_byte_in(0x60);

  if (scancode == LSHIFT_DOWN || scancode == RSHIFT_DOWN) {
    shift_down = true;
    return;
  } else if (scancode == LSHIFT_UP || scancode == RSHIFT_UP) {
    shift_down = false;
    return;
  }

  if (scancode > SC_MAX) return;

  char letter = shift_down ? sc_asccii_upper[(int)scancode] : sc_ascii_lower[(int)scancode];

  if (char_handler != 0 && letter != '\0') {
    char_handler(letter);
  }
}

void init_keyboard(void (*ch)(char)) {
  char_handler = ch;
  register_interrupt_handler(IRQ1, keyboard_callback);
}

