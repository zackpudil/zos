global _start
[bits 32]
[extern kmain]
_start:
  call kmain
  jmp $
