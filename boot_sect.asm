
[org 0x7c00]

KERNEL_OFFSET equ 0x1000
	
mov [BOOT_DRIVE], dl	; BIOS 将启动盘保存在dl中  

mov bp, 0x9000		; 设置栈
mov sp, bp

mov bx, MSG_REAL_MODE  	; 打印进入16-bit模式
call print_string

call load_kernel	; 加载内核

call switch_to_pm	; 切换到32-bit模式, 无返回

jmp $

%include "print.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "print_string_pm.asm"
%include "32bit-switch.asm"

[bits 16]

load_kernel:
	mov bx, MSG_LOAD_KERNEL		; 打印正在加载内核
	call print_string

	mov bx, KERNEL_OFFSET		; 设置从磁盘加载参数，从磁盘读取保存在 0x1000
	mov dh, 5 			; 从磁盘加载2个扇区（包括引导扇区）到内存地址 KERNEL_OFFSET
	mov dl, [BOOT_DRIVE]
	call disk_load
	
	;mov dx, [KERNEL_OFFSET]
	;call print_ln
	;call print_hex

	ret

[bits 32]

BEGIN_PM:				; 切换到保护模式和初始化完成调用此方法
	mov ebx, MSG_PROT_MODE		; 使用32-bit模式打印成功进入保护模式
	call print_string_pm

	call KERNEL_OFFSET		; 跳转到加载内核代码的地址

	jmp $

BOOT_DRIVE	db 0
MSG_REAL_MODE	db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE 	db "Successfully landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0

times 510-($-$$) db 0
dw 0xaa55

