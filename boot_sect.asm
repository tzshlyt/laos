loop:
	jmp loop

times 510-($-$$) db 0   ; $：当前地址，$$: 当前段的起始地址， 510: 512-2个字节

; db dw dd 伪汇编指令(告诉汇编程序如何进行汇编的指令，为汇编程序所识别并指导汇编如何进行)
; db (define byte)  1字节 x86 32-bit
; dw (define word)	2字节
; ds (define double word) 4字节

dw 0xaa55  				; 启动盘标识
