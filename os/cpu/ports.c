#include "ports.h"
/*
 *  	mov dx, 0x3f2		; Must use DX to store port address
 *  	in al, dx		; Read contnets of port (i.e. DOR) to AL
 *  	or al, 00001000b 	; Switch on the motor bit
 *  	out dx, al		; Update DOR of the device
 */

uint8_t port_byte_in(uint16_t port) {
	uint8_t result;
	// Notice how the source and destination registers are switched from NASM
	// '"=a" (result)'; set '=' the C variable '(result)' to the value of register e'a'x
	// '"d" (port)': map the C variable '(port)' into e'd'x register
	asm("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_byte_out(uint16_t port, uint8_t data) {
	// "a" (data) means: load EAX with data
	// "d" (port) means: load EDX with port
	asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in(uint16_t port) {
	uint16_t result;
	asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void port_word_out(uint16_t port, uint16_t data) {
	asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}
