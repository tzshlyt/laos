[bits 16]

switch_to_pm:
	cli						; 1. 先关闭中断, 因为 32-bit 模式不在使用 BIOS 中断
	lgdt [gdt_descriptor]	; 2. 加载GDT描述
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax			; 3. cr0 置位, 切换到 32-bit 模式
	jmp CODE_SEG:init_pm   	; 4. 长跳转, 使用不同的 segment

[bits 32]
init_pm:
	mov ax, DATA_SEG 		; 5. 更新 segment 寄存器
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000 		; 6. 更新栈, 在顶部空闲的内存空间，栈是向下增长的
	mov esp, ebp

	call BEGIN_PM			; 7. 打印初始化完成, 调用内核函数进入内核