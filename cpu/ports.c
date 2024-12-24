#include "ports.h"

u8 port_byte_in(u16 port) {
  u8 result;
  asm("inb %1, %0" : "=a" (result) : "dN" (port));
  return result;
}

void port_byte_out(u16 port, u8 data) {
  asm volatile("outb %0, %1" : : "a" (data), "dN" (port));
}

u16 port_word_in(u16 port) {
  u16 result;
  asm("inw %1, %0" : "=a" (result) : "dN" (port));

  return result;
}

void port_word_out(u16 port, u16 data) {
  asm volatile("outw %0, %1" : : "a" (data), "dN" (port));
}

u32 port_dword_in(u16 port) {
  u32 result;
  asm("inl %1, %0" : "=a" (result) : "dN" (port));

  return result;
}

void port_dword_out(u16 port, u32 data) {
  asm volatile("outl %0, %1" : : "a" (data), "dN" (port));
}
