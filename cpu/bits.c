#include "bits.h"

u16 switch_endian_16(u16 val) {
  return (val << 8) | (val >> 8);
}

u32 switch_endian_32(u32 val) {
  return (val << 24)
    | ((val << 8) & 0x00FF0000)
    | ((val >> 8) & 0x0000FF00)
    | (val >> 24);
}
