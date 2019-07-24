; 从启动盘读取扇区的数据
[org 0x7c00]

DATA_OFFSET equ 0x9000

mov [BOOT_DRIVE], dl	; BIOS 把启动设备保存在DL寄存其中	

mov bp, 0x8000   	; 栈起始地址
mov sp, bp

mov bx, DATA_OFFSET 	; 读取5个扇区的数据到内存 0x0000(es):0x9000(bx) 中
mov dh, 5
mov dl, [BOOT_DRIVE]
call disk_load

mov dx, [DATA_OFFSET]	; 打印第一个扇区第一个词
call print_hex

call print_ln

mov dx, [DATA_OFFSET + 512]  ; 打印第二个扇区第一个词
call print_hex

jmp $

BOOT_DRIVE:
	db 0

%include "print.asm"
%include "disk_load.asm"

times 510 - ($-$$) db 0
dw 0xaa55

times 256 dw 0xdada
times 256 dw 0xface
