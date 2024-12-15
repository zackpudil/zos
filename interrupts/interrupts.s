[extern isr_handler]
[extern irq_handler]

isr_common_stub:
  pusha
  mov ax, ds
  push eax

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  call isr_handler

  pop eax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  popa
  add esp, 8
  sti
  iret

irq_common_stub:
  pusha
  mov ax, ds

  push eax
  mov ax, 0x10
  mov ds, ax,
  mov es, ax
  mov fs, ax
  mov gs, ax

  call irq_handler

  pop ebx
  mov ds, bx
  mov es, bx
  mov fs, bx
  mov gs, bx
  popa

  add esp, 8
  sti
  iret


 
%macro isr 1
[global isr%1]
isr%1:
  cli
  push byte 0
  push byte %1
  jmp isr_common_stub
%endmacro

isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
isr 8
isr 9
isr 10
isr 11
isr 12
isr 13
isr 14
isr 15
isr 16
isr 17
isr 18
isr 19
isr 20
isr 21
isr 22
isr 23
isr 24
isr 25
isr 26
isr 27
isr 28
isr 29
isr 30
isr 31

%macro irq 1
[global irq%1]
irq%1:
  cli
  push byte %1
  push byte 32 + %1
  jmp irq_common_stub
%endmacro

irq 0
irq 1
irq 2
irq 3
irq 4
irq 5
irq 6
irq 7
irq 8
irq 9
irq 10
irq 11
irq 12
irq 13
irq 14
irq 15
