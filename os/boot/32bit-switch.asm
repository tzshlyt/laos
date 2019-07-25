[bits 16]

switch_to_pm:
	cli			; 关闭中断	
	lgdt [gdt_descriptor]	; 加载GDT描述
	mov eax, cr0
	or eax, 0x1		
	mov cr0, eax		; 设置cr0 为1
	jmp CODE_SEG:init_pm   	; 长跳转

[bits 32]
init_pm:
	mov ax, DATA_SEG 	; 更新段寄存器
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000 	; 更新栈
	mov esp, ebp
	
	call BEGIN_PM		; 打印初始化完成	
