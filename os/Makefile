
# 批量匹配c文件名 kernel.c --> kernel
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

# kernel.c --> kernel.o 
OBJ = ${C_SOURCES:.c=.o}

CC = /usr/local/bin/i386-elf-gcc

# -g: Use debugging symbols in gcc
CFLAGS = -g

%.bin: %.asm
	nasm $< -f bin -o $@


all: os-image

run: all
	bochs

os-image: boot/boot_sect.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@



clean:
	rm -rf *.bin *.dis *.o *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o
	rm os-image
