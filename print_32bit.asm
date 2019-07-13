
[org 0x7c00]

mov ebx, HELLO_MSG
call print_string_pm


mov ebx, GOODBY_MSG
call print_string_pm

jmp $

%include "print.asm"
%include "print_no_bios.asm"

HELLO_MSG:
	db "hello, world!", 0

GOODBY_MSG:
	db "Goodbye!", 0

times 510-($-$$) db 0
dw 0xaa55
