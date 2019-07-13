
[org 0x7c00]

mov bx, HELLO_MSG
call print_string
call print_ln

mov bx, GOODBY_MSG
call print_string
call print_ln

mov dx, 0x1fb6
call print_hex


jmp $

%include "print_string.asm"

HELLO_MSG:
	db "hello, world!", 0

GOODBY_MSG:
	db "Goodbye!", 0

HEX_OUT:
	db '0x0000', 0

print_hex:
	pusha
	mov cx, 0 ; index变量

hex_loop:
	cmp cx, 4
	je end1

	mov ax, dx
	and ax, 0x000f
	add al, 0x30
	cmp al, 0x39
	jle setp2
	add al, 7
	
setp2:
	mov bx, HEX_OUT + 5
	sub bx, cx 
	mov [bx], al
	ror dx, 4
	
	add cx, 1
	jmp hex_loop

end1:
	mov bx, HEX_OUT
	call print_string

	popa
	ret 


times 510-($-$$) db 0
dw 0xaa55
