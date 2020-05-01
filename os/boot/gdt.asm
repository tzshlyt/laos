; GDT 配置
; 内核代码具有最高访问权限，所以可以从访问 0 - 4G
; 分段和分页是 x86 内存保护的两种方式，分页更有优势，分段快过时了，但是内置在 x86 结构中,需要设置为flat memory model
; 分段能做到分页不能做到的是分段可以设置代码权限等级，0 <- 内核模式，3 <- 用户模式
; In the x86, we have 6 segmentation registers. Each holds an offset into the GDT. They are cs (code segment),
; ds (data segment), es (extra segment), fs, gs, ss (stack segment).
; The code segment must reference a descriptor which is set as a 'code segment'.
; There is a flag for this in the access byte. The rest should all reference a descriptor which is set as a 'data segment'.
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

gdt_descriptor:						; GDT 描述符入口，全局描述符表寄存器（GDTR）
	dw gdt_end - gdt_start - 1 		; gdt 大小 (16 bit), 因为 gdt_end 相当于下一个起始地址,所以需要 -1，最多可以创建 2^16 / 8 = 8192 个描述符
	dd gdt_start					; gdt 地址 (32 bit)

; 定义常量，为了计算方便
CODE_SEG equ gdt_code - gdt_start	; 代码段偏移
DATA_SEG equ gdt_data - gdt_start	; 数据段偏移
