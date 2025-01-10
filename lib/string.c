#include "string.h"
#include "mem.h"

static char *s =  (char *)0;

bool str_begins_with(char *starts, char *test, u8 start_len) {
  for(u8 i = 0; i < start_len; i++) {
    if (starts[i] != test[i])  return false;
  }

  return true;
}

char *number_to_string(u8 number) {
  if (!s) {
    s = (char *)malloc(sizeof(char)*9, true, 0);
  }
  mset((u8 *)s, (u8)'\0', 9);

  if (number == 0) {
    s[0] = '0';
    return s;
  }

  u8 i = 0;
  while (number > 0) {
    s[i++] = number % 10 + '0';
    number /= 10;
  }

  for (u8 j = 0, k = i - 1; j < k; j++, k--) {
    char temp = s[j];
    s[j] = s[k];
    s[k] = temp;
  }

  return s;
}

char **split(char *str, char c) {
  char **strs = (char **)malloc(sizeof(char *)*10, false, 0);
  
  strs[0] = (char *)malloc(sizeof(char)*10, false, 0);
  u8 j = 0;
  u8 k = 0;

  for(u8 i = 0; i < str_len(str); i++) {
    if (str[i] == c) {
      strs[j][k] = '\0';
      j++;
      k = 0;
      strs[j] = (char *)malloc(sizeof(char)*10, false, 0);
    } else {
      strs[j][k] = str[i];
      k++;
    }
  }

  return strs;
}

bool is_ip(char *str) {
  if (str[0] - 48 >= 10) return false;
  if (str[1] != '.' && str[1] - 48 >= 10) return false;
  if (str[2] != '.' && str[2] - 48 >= 10) return false;

  return true;
}

/* THE MOST HACKIEST AND DUMMEST SHIT I'VE EVER DONE*/
u8 str_to_number(char *str) {
  u8 ret = 0;
  if (str_len(str) == 3) {
    ret += (str[0] - 48)*100;
    ret += (str[1] - 48)*10;
    ret += (str[2] - 48)*1;
  } else if(str_len(str) == 2) {
    ret += (str[0] - 48)*10;
    ret += (str[1] - 48)*1;
  } else if (str_len(str) == 1) {
    ret += (str[0] - 48)*1;
  }

  return ret;
}

char *data_to_str(u32 c, u8 nibbles) {
  if (!s) {
    s = (char *)malloc(sizeof(char)*9, true, 0);
  }

  mset((u8 *)s, (u8)'\0', 9);

  for(u8 i = 0; i < nibbles; i++) {
    s[nibbles - 1 - i] = nibble_to_char(c >> (i*4) & 0x0F);
  }

  return s;
}

char *dword_to_str(u32 c) {
  return data_to_str(c, 8);
}

char *word_to_str(u16 c) {
  return data_to_str((u32)c, 4);
}

char *byte_to_str(u8 c) {
  return data_to_str((u32)c, 2);
}

u32 str_to_data(char *s, u8 nibs) {
  u32 res = 0x0;

  for(u8 i = 0; i < nibs; i++) {
    u32 nib = (u32)char_to_nibble(s[i]);
    res += (nib << ((nibs/2) - i)*4);
  }

  return res;
}

u32 str_to_dword(char *s) {
  return str_to_data(s, 8);
}

u16 str_to_word(char *s) {
  return (u16)str_to_data(s, 4);
}

u8 str_to_byte(char *s) {
  return (u8)str_to_data(s, 2);
}

u32 str_len(char *s) {
  u32 i = 0;
  while (s[i] != '\0') {
    i++;
  }

  return i;
}


u8 char_to_nibble(char c) {
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

char nibble_to_char(u8 i) {
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
