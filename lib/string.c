#include "string.h"

bool str_begins_with(char *starts, char *test, u8 start_len) {
  for(u8 i = 0; i < start_len; i++) {
    if (starts[i] != test[i])  return false;
  }

  return true;
}

u8 char_to_num(char c) {
  switch(c) {
    case '0': return 0x0;
    case '1': return 0x1;
    case '2': return 0x2;
    case '3': return 0x3;
    case '4': return 0x4;
    case '5': return 0x5;
    case '6': return 0x6;
    case '7': return 0x7;
    case '8': return 0x8;
    case '9': return 0x9;
    case 'A': 
    case 'a':
      return 0xa;
    case 'B':
    case 'b':
      return 0xb;
    case 'C':
    case 'c':
      return 0xc;
    case 'D':
    case 'd':
      return 0xd;
    case 'E':
    case 'e':
      return 0xe;
    case 'F': 
    case 'f':
      return 0xf;
    default: return 0;
  }
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
