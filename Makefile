C_SOURCES=$(wildcard kernel/*.c interrupts/*.c cpu/*.c lib/*.c drivers/*.c)
OBJ=${C_SOURCES:.c=.o}
WSL_PATH=/mnt/c/Users/zackp

all: run

%.o: %.s
	nasm -f elf $< -o $@

%.o: %.c 
	gcc -g -m32 -Wall -ffreestanding -fno-pie -c $< -o $@

debug.elf: kernel/entry.o interrupts/interrupts.o ${OBJ} 
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^

boot.bin: boot/boot.asm
	nasm -f bin $< -o $@

kernel.bin: kernel/entry.o interrupts/interrupts.o ${OBJ} 
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

zos.bin: boot.bin kernel.bin
	cat $^ > $@

run: zos.bin
	cp $^ ${WSL_PATH}/$^
	cmd.exe /c "qemu-system-i386.exe ^ \
		-fda $$(wslpath -w ${WSL_PATH}/$^) ^ \
		-vga std ^ \
		-s -d guest_errors"

debug: debug.elf
	gdb -ex "target remote $$(cat /etc/resolv.conf | tail -1 | sed 's/.* //'):1234" \
			-ex "symbol-file $^" 

clean:
	$(RM) *.bin *.elf *.o **/*.bin **/*.elf **/*.o ${WSL_PATH}/*.bin
