#include "idt.h"

void set_idt_gate(int n, u32 handler) {
  idt[n].low_offset = (u16)(handler & 0xffff);
  idt[n].sel = KERNEL_CS;
  idt[n].always_zero = 0;
  idt[n].flags = 0x8e;
  idt[n].high_offset = (u16)((handler >> 16) & 0xffff);
}

void set_idt() {
  idt_reg.base = (u32)&idt;
  idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;

  asm volatile("lidtl (%0)" : : "r" (&idt_reg));
}
