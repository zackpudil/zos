#include "string.h"

bool str_begins_with(char *starts, char *test, u8 start_len) {
  for(u8 i = 0; i < start_len; i++) {
    if (starts[i] != test[i])  return false;
  }

  return true;
}
