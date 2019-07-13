; load dh sectors to ex:bx from drive dl
disk_load:
	pusha

	push dx
	
	mov ah, 0x02 	; read disk function
	mov al, dh 	; read dh sectiors
	mov ch, 0x00 	; cylinder 0
	mov dh, 0x00 	; head number
	mov cl, 0x02 	; start reading 2nd sector, 0x01 is our boot sector 

	int 0x13

	jc disk_error

	pop dx
		
	cmp al, dh	
	jne sectors_error 	
	popa
	ret

disk_error: 
	mov bx, DISK_ERROR_MSG
	call print_string
	call print_ln
	mov dh, ah 	; ah = error code, dl = disk drive that dropped the error
	call print_hex
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
