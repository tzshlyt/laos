
print_string:
	pusha
	
start:
	mov al, [bx]	
	cmp al, 0
	je end

	mov ah, 0x0e
	int 0x10
	
	add bx, 1

	jmp start
	
end:
	popa
	ret

print_ln:
	pusha
	mov ah, 0x0e
    	mov al, 0x0a ; newline char
    	int 0x10
    	mov al, 0x0d ; carriage return
    	int 0x10
	popa
	ret

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

HEX_OUT:
	db '0x0000', 0

