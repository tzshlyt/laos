
# $@ = target file
# $< = first dependency
# $^ = all dependencies

# 批量匹配c文件名, 如 kernel.c --> kernel
# c 文件
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
# 头文件
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)


# object 文件，.c 文件 --> .o 文件
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)   	# linux 系统
	CC = gcc
	GDB = gdb
	LD = ld
else
	CC = /usr/local/bin/i386-elf-gcc
	GDB = /usr/local/bin/i386-elf-gdb
	LD = /usr/local/bin/i386-elf-ld
endif

# -g: Use debugging symbols in gcc
# -fno-exceptions: 禁用异常机制
CFLAGS = -g -Wall -Wextra -fno-exceptions -m32

# 生成内核镜像
os-image.bin: boot/boot_sect.bin kernel.bin
	cat $^ > $@

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	${LD} -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	${LD} -m elf_i386 -o $@ -Ttext 0x1000 $^

run: os-image.bin
# 	bochs -f ../bochsrc.txt        # 使用 bochs 启动
	qemu-system-i386 -fda os-image.bin	# 使用 qemu 启动

# Open the connection to qemu and load our kernel-object file with symbols
#	备份配置
#	qemu-system-i386 -s -fda os-image -d guest_errors,int &
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o *.elf os-image.bin
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o
