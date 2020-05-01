; GDT 配置
; 内核代码具有最高访问权限，所以可以从访问 0 - 4G
gdt_start:

gdt_null:		; 第一个条目强制设置为null, 偏移 0x00
	dd 0x0		; dd 是4个字节
	dd 0x0

; GDT for code segment. base = 0x00000000, length = 0xfffff
gdt_code:			; 代码段描述符, 偏移 0x08
	dw 0xffff		; gdt配置, 具体看文档, segment length，bits 0-15
	dw 0x0			; segment base, bits 0-15
	db 0x0			; segment base, bits 16-23
	db 10011010b	; flags (8 bits)
	db 11001111b	; flags (4 bits) + segment length, bits 16-19
	db 0x0			; segment base, bits 24-31

gdt_data:		;数据段描述符, 偏移 0x10
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0

gdt_end:		; 为了计算方便

gdt_descriptor:						; GDT 描述符入口
	dw gdt_end - gdt_start - 1 		; gdt 大小 (16 bit), -1 , 因为 gdt_end 相当于下一个起始地址
	dd gdt_start					; gdt 地址 (32 bit)

; 定义常量，为了计算方便
CODE_SEG equ gdt_code - gdt_start	; 代码段偏移
DATA_SEG equ gdt_data - gdt_start	; 数据段偏移
