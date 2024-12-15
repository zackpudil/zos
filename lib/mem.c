#include "mem.h"

void mcopy(u8 *src, u8 *dest, u32 nbytes) {
  for(int i = 0; i < nbytes; i++) {
    *(dest + i) = *(src + i);
  }
}

void mset(u8 *dest, u8 val, u32 len) {
  u8 *temp = (u8 *)dest;
  for ( ; len != 0; len--) *temp++ = val;
}

u32 free_mem_addr = 0x10000;

u32 malloc(u32 size, bool align, u32 *phys_addr) {
  if (align == true && (free_mem_addr & 0xFFFFF000)) {
    free_mem_addr &= 0xFFFFF000;
    free_mem_addr += 0x1000;
  }

  if (phys_addr) *phys_addr = free_mem_addr;

  u32 ret = free_mem_addr;
  free_mem_addr += size;
  return ret;
}

