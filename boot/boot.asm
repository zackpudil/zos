[bits 16]
[org 0x7c00]

KERNEL_ADDR equ 0x1000

start:
  ; initialize stack pointer
  mov bp, 0x9000
  mov sp, bp

  mov bx, LOADING_MSG
  call print

  mov dh, 54
  call load_kernel

  call switch

  ; should never get here
  hlt

print:
  pusha
  mov ah, 0x0e
.loop:
  mov al, [bx]
  cmp al, 0

  je .end

  int 0x10

  add bx, 1

  jmp .loop

.end:
  popa
  ret

load_kernel:
  pusha
  push dx

  mov ah, 0x02 ; command
  mov al, dh   ; # sectors to read
  mov ch, 0x00 ; cylinder
  mov cl, 0x02 ; start Sector
  mov dh, 0x00 ; head
  mov dl, 0x00 ; drive

  mov bx, KERNEL_ADDR ; buffer

  int 0x13

  jc .disk_error

  pop dx
  cmp al, dh
  jne .sectors_error

  popa
  ret

.disk_error:
  mov bx, DISK_ERROR
  call print
  jmp $

.sectors_error:
  mov bx, SECTOR_ERROR
  call print
  jmp $

switch:
  ; set video mode to 320x200 256 colors
  mov ax, 0x0013
  int 0x10

  cli               ; clear interrupts
  lgdt [gdt_desc]   ; load global descriptor table

  mov eax, cr0      
  or eax, 0x1
  mov cr0, eax

  jmp CODE_SEG:start_32

[bits 32]
start_32:
  ; initailze all registers
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  ; initialize new 32-bit stack pointer
  mov ebp, 0x90000
  mov esp, ebp

  ; call kernel (see kernel/entry.s)
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

LOADING_MSG db "Loading the kernel...", 0x0a, 0x0d, 0
DISK_ERROR db "Disk read error", 0x0a, 0x0d, 0
SECTOR_ERROR db "Incorrect number of sectors read", 0x0a, 0x0d, 0

times 510-($-$$) db 0
dw 0xAA55
