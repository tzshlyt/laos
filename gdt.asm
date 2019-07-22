; GDT
gdt_start:

gdt_null:		; 第一个条目强制设置为null
	dd 0x0		; 4字节
	dd 0x0

gdt_code:		;代码段描述符
	dw 0xffff	;gdt配置, 具体看文档
	dw 0x0
	db 0x0
	db 10011010b
	db 11001111b
	db 0x0

gdt_data:		;数据段描述符
	dw 0xffff
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0

gdt_end:		; 为了计算

gdt_descriptor:				; GDT 描述符号
	dw gdt_end - gdt_start - 1 	; gdt 大小，-1 , 因为gdt_end 相当于下一个起始地址
	dd gdt_start			; gdt 地址

CODE_SEG equ gdt_code - gdt_start	; 代码段偏移，定义常量，为了计算方便
DATA_SEG equ gdt_data - gdt_start	; 数据段偏移
