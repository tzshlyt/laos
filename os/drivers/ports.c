/*
 *  	mov dx, 0x3f2		; Must use DX to store port address
 *  	in al, dx		; Read contnets of port (i.e. DOR) to AL
 *  	or al, 00001000b 	; Switch on the motor bit
 *  	out dx, al		; Update DOR of the device
 */

unsigned char port_byte_in(unsigned short port) {
	unsigned char result;
	// Notice how the source and destination registers are switched from NASM
	// '"=a" (result)'; set '=' the C variable '(result)' to the value of register e'a'x
	// '"d" (port)': map the C variable '(port)' into e'd'x register
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
	// "a" (data) means: load EAX with data
	// "d" (port) means: load EDX with port
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port) {
	unsigned short result;
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void port_word_out(unsigned short port, unsigned short data) {
	__asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
