// I/O 操作不能使用 c 语言，所以使用内联汇编
// 因为 I/O 操作耗时占用 CPU，所以使用 DMA (direct memory access)
#include "ports.h"
/*
 *		I/O 操作的例子
 *  	mov dx, 0x3f2		; Must use DX to store port address
 *  	in al, dx			; Read contnets of port (i.e. DOR) to AL
 *  	or al, 00001000b 	; Switch on the motor bit
 *  	out dx, al			; Update DOR of the device
 */

// 从 I/O 读取 1 byte 数据
uint8_t port_byte_in(uint16_t port) {
	uint8_t result;
	// Notice how the source and destination registers are switched from NASM, 和 nasm 语法相反，即把前面的值赋给后面
	// '"=a" (result)'; set '=' the C variable '(result)' to the value of register e'a'x
	// '"d" (port)': map the C variable '(port)' into e'd'x register
	// %表示寄存器，因为 c 语言中 % 是转义字符, 所以使用 %%
	//
	asm("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

// 写 1 byte 数据到 I/O
void port_byte_out(uint16_t port, uint8_t data) {
	// "a" (data) means: load EAX with data
	// "d" (port) means: load EDX with port
	asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

// 从 I/O 读取 1 word 数据
uint16_t port_word_in(uint16_t port) {
	uint16_t result;
	asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

// 写 1 word 数据到 I/O
void port_word_out(uint16_t port, uint16_t data) {
	asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}
