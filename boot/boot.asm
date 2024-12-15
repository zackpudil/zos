[bits 16]
[org 0x7c00]

KERNEL_ADDR equ 0x1000

start:
  mov bp, 0x9000
  mov sp, bp

  mov ax, 0x0013
  int 0x10

  call load_kernel
  call switch

  hlt

load_kernel:
  pusha

  mov ah, 0x02 ; command
  mov al, 0x20 ; # sectors to read
  mov ch, 0x00 ; cylinder
  mov cl, 0x02 ; start Sector
  mov dh, 0x00 ; head
  mov dl, 0x00 ; drive

  mov bx, KERNEL_ADDR ; buffer

  int 0x13

  jnc .done
  mov ah, 0x0E
  mov al, 'E'
  int 0x10
  jmp $

.done:
  popa
  ret

switch:
  cli
  lgdt [gdt_desc]
  mov eax, cr0
  or eax, 0x1
  mov cr0, eax

  jmp CODE_SEG:start_32

[bits 32]
start_32:
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebp, 0x90000
  mov esp, ebp

  call KERNEL_ADDR
  jmp $

gdt_start:
  dd 0x0
  dd 0x0

gdt_code:
  dw 0xffff
  dw 0x0
  db 0x0
  db 10011010b
  db 11001111b
  db 0x0

gdt_data:
  dw 0xffff
  dw 0x0
  db 0x0
  db 10010010b
  db 11001111b
  db 0x0

gdt_end:

gdt_desc:
  dw gdt_end - gdt_start - 1
  dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

.done:
  popa
  ret


times 510-($-$$) db 0
dw 0xAA55
