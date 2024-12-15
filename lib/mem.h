#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"

void mcopy(u8 *source, u8 *dest, u32 nbytes);
void mset(u8 *dest, u8 val, u32 len);

u32 malloc(u32 size, bool align, u32 *phys_addr);

#endif
