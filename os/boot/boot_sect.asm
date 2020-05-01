; 引导程序

[org 0x7c00]				; 声明引导程序加载内存地址

KERNEL_OFFSET equ 0x1000	; 内核代码加载的内存地址, 与链接到内核使用的相同，
							; ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat binary

mov [BOOT_DRIVE], dl	; 记住 BIOS 将启动盘设备号保存在 dl 中

mov bp, 0x9000			; 设置栈，设置到一个安全的位置
mov sp, bp

mov bx, MSG_REAL_MODE  	; 打印信息，进入16-bit模式
call print_string
call print_ln

call load_kernel	; 从磁盘加载内核

call switch_to_pm	; 关闭中断，加载 GDT 等等，切换到32-bit模式, 无返回, 最后调用 BEGIN_PM

jmp $				; 永不执行

%include "boot/print.asm"
%include "boot/disk_load.asm"
%include "boot/gdt.asm"
%include "boot/print_string_pm.asm"
%include "boot/32bit-switch.asm"

[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL		; 打印信息, 正在加载内核
	call print_string
	call print_ln

	mov bx, KERNEL_OFFSET		; 从磁盘读取数据保存到内存中 bx (0x1000)
	mov dh, 31 					; dh <- 读取扇区数，不包括引导扇区 (因为在disk_load函数中设置了从第二个扇区开始读), 可以使用更大的空间，因为内核代码可能很大
								; 很可能引起莫名其妙bug
	mov dl, [BOOT_DRIVE]  		; dl <- 磁盘设备号
	call disk_load

	;mov dx, [KERNEL_OFFSET]   ; 调试打印
	;call print_ln
	;call print_hex

	ret

[bits 32]
BEGIN_PM:						; 切换到保护模式和初始化完成调用此方法
	mov ebx, MSG_PROT_MODE		; 打印信息，使用32-bit模式打印成功进入保护模式
	call print_string_pm

	call KERNEL_OFFSET			; 跳转到加载内核代码的地址，将控制器交给内核

	jmp $						; 如果内核将控制权交回时停留在这里，如果有的话

BOOT_DRIVE	db 0
MSG_REAL_MODE	db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE 	db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0

; 填充
times 510-($-$$) db 0
dw 0xaa55