#include "ports.h"
/*
 *  	mov dx, 0x3f2		; Must use DX to store port address
 *  	in al, dx		; Read contnets of port (i.e. DOR) to AL
 *  	or al, 00001000b 	; Switch on the motor bit
 *  	out dx, al		; Update DOR of the device
 */

u8 port_byte_in(u16 port) {
	u8 result;
	// Notice how the source and destination registers are switched from NASM
	// '"=a" (result)'; set '=' the C variable '(result)' to the value of register e'a'x
	// '"d" (port)': map the C variable '(port)' into e'd'x register
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_byte_out(u16 port, u8 data) {
	// "a" (data) means: load EAX with data
	// "d" (port) means: load EDX with port
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

u16 port_word_in(u16 port) {
	u16 result;
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void port_word_out(u16 port, u16 data) {
	__asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
