; load dh sectors to ex:bx from drive dl
; 从设备 dl 中读取 dh 个扇区到 ex:bx 所指的内存地址
disk_load:
	pusha

	push dx

	mov ah, 0x02 	; ah <- int 0x13, 0x02 == 'read'
	mov al, dh 		; al <- 读取多少个扇区 (0x01 .. 0x80)
	mov cl, 0x02 	; cl <- 第几个扇区开始读 (0x01 ... 0x11)
					; 0x01 是引导扇区，0x02是第一个有用的扇区
	mov ch, 0x00 	; ch <- 柱面 (0x0 .. 0x3FF, )
	; dl <- 设备号，调用者做为参数调用，并在 BIOS 中获取
	; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
	mov dh, 0x00 	; dh <- 第几个磁头 (0x0 .. 0xF)

	; [ex: bx] <- 数据将要保存的地址
	; 调用者做为参数设置

	int 0x13		; BIOS 中断

	jc disk_error	; 如果carry flag置位，说明有磁盘错误

	pop dx

	cmp al, dh		; BIOS设置实际读取的扇区数到 al, != dh 说明错误
	jne sectors_error
	popa
	ret

disk_error:
	mov bx, DISK_ERROR_MSG 		; 打印信息，磁盘读取错误
	call print_string
	call print_ln

	mov dh, ah 			; ah = error code, dl = disk drive that dropped the error
	call print_hex 		; 检查错误码 https://stanislavs.org/helppc/int_13-1.html
	jmp disk_loop

sectors_error:
	mov bx, SECTORS_ERROR
	call print_string

disk_loop:
	jmp $

DISK_ERROR_MSG:
	db "Disk read error!", 0
SECTORS_ERROR:
	db "Incorrect number of sectors read", 0
