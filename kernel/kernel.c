#include "../cpu/types.h"
#include "../interrupts/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/video.h"
#include "../lib/print.h"

static u8 screen_color = 0x01;
static void user_input(char input);
char num_to_char(u8 i);

void kmain() {
  isr_install();
  irq_install();

  init_keyboard(user_input);

  video_init();
  clear_screen(0);

  print_str("256 COLOR PALLET: \n", 0x1f);
  u8 currentColor = 0;
  for(u8 i = 0; i < 16; i++) {
    print_char(num_to_char(i), 0x1f);
    print_str("X: ", 0x1f);
    for(int i = 0; i < 16; i++) {
      print_char(num_to_char(i), currentColor);
      currentColor++;
    }
    print_char('\n', 0x00);
  }

  print_str("TYPE ANYTHING: \n", 0x1f);

  set_display_buffer();
  video_draw();
}

char num_to_char(u8 i) {
  switch(i) {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return 'A';
    case 11: return 'B';
    case 12: return 'C';
    case 13: return 'D';
    case 14: return 'E';
    case 15: return 'F';
    default: return 'X';
  }
}

static void user_input(char input) {
  print_char(input, screen_color);
  screen_color++;
  
  set_display_buffer();
  video_draw();
}

