; 汇编 if 实验

;[org 0x7c00]

mov ah, 0x0e

mov bx, 30
cmp bx, 4
jle bx_less_equal_4
cmp bx, 40
jl bx_less_40
jmp else
jmp endif

bx_less_equal_4:
	mov al, 'A'
	jmp endif

bx_less_40:
	mov al, 'B'
	jmp endif
else:
	mov al, 'C'
	jmp endif

endif:
	int 0x10


jmp $


times 510-($-$$) db 0
dw 0xaa55


