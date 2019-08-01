
# 批量匹配c文件名 kernel.c --> kernel
C_SOURCES = $(wildcard kernel/*.c drivers/*.c) 
HEADERS = $(wildcard kernel/*.h drivers/*.h)

# kernel.c --> kernel.o 
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}

CC = /usr/local/bin/i386-elf-gcc
GDB = /usr/local/bin/i386-elf-gdb

# -g: Use debugging symbols in gcc
CFLAGS = -g

run: os-image
# 	bochs -f ../bochsrc.txt        # bochs 启动
	qemu-system-i386 -fda os-image	# qemu 启动

os-image: boot/boot_sect.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image kernel.elf
	qemu-system-i386 -s -fda os-image &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o
	rm ../*.lock
