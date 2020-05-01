; 32-bit 保护模式打印字符串
; 因为此时 16-bit 的 BIOS 中断方式打印字符串方式不可用

[bits 32] 			; 使用 32-bit 保护模式

VIDEO_MEMORY equ 0xb8000    ; 内存映射地址
WHITE_ON_BLACK equ 0x0f		; 字体颜色

print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY

print_string_pm_loop:
	mov al, [ebx]           ; [ebx] 字符的地址
	mov ah, WHITE_ON_BLACK

	cmp al, 0				; 字符串结束
	je print_string_pm_done

	mov [edx], ax           ; 保存字符和属性到屏幕内存
	add ebx, 1				; 下一个字符
	add edx, 2				; 下一个屏幕内存地址

	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret
