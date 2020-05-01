# $@ = target file
# $< = first dependency
# $^ = all dependencies

all: os-image

run: all
	qemu-system-i386 -fda os-image

#
kernel.bin: kernel_entry.o kernel.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel_entry.o : kernel_entry.asm
	nasm $< -f elf -o $@

kernel.o : kernel.c
	gcc -m32 -ffreestanding -c $< -o $@

#
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

boot_sect.bin: boot_sect.asm
	nasm $< -f bin -o $@

os-image: boot_sect.bin kernel.bin
	cat $^ > $@

clean:
	rm  *.bin *.o *.dis os-image *.map

