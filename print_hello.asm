
[org 0x7c00]

mov bx, HELLO_MSG
call print_string

call print_ln

mov bx, GOODBY_MSG
call print_string

jmp $

%include "print_string.asm"

HELLO_MSG:
	db "hello, world!", 0

GOODBY_MSG:
	db "Goodbye!", 0

times 510-($-$$) db 0
dw 0xaa55
