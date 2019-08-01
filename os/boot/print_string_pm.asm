[bits 32] 			; 使用32-bit 保护模式

VIDEO_MEMORY equ 0xb8000    ; 
WHITE_ON_BLACK equ 0x0f		; 字体颜色

print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY

print_string_pm_loop:
	mov al, [ebx]           ; [ebx] 字符的地址
	mov ah, WHITE_ON_BLACK
	
	cmp al, 0				; 字符串结束
	je print_string_pm_done

	mov [edx], ax           ; 保存字符和属性到视频内存
	add ebx, 1				; 下一个字符
	add edx, 2				; 下一个视频内存地址

	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret
