; Defined in isr.c
[extern isr_handler]
[extern irq_handler]

; Common ISR code
; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
; 所有中断处理函数共有的保护现场操作
isr_common_stub:
	; 1. Save CPU state
	pusha		        ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
	mov ax, ds	        ; Lower 16-bits of eax = ds, 保存数据段描述符
	push eax 	        ; save the data segment descriptor

	mov ax, 0x10	    ; kernel data segment descriptor,  加载内核数据段描述符表
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    push esp            ; registers_t *r, 此时的 esp 寄存器的值等价于 pt_regs 结构体的指针

	; 2. Call C handler
	cld                 ; C code following the sysV ABI requires DF to be clear on function entry
    call isr_handler    ; 在 C 语言代码里

    ; 3. Restore state
	pop esp
    pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa                ; Pops edi, esi, ebp, esp, ebx, edx, ecx, eax
	add esp, 8	        ; cleans up the pushed error code and pushed ISR number, 清理栈里的 error code 和 ISR
	;sti                ; cli is redundant, since we already established on the IDT entries if interrupts are enabled within a handler using the idt_gate_t flags
                        ; sti is also redundant, as iret loads the eflags value from the stack, which contains a bit telling whether interrupts are on or off.
                        ;In other words the interrupt handler automatically restores interrupts whether or not interrupts were enabled before this interrupt

	;The IRET instruction is specifically designed to return from an interrupt
	iret		        ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESp


; Common IRQ code. Identical to ISR code except for the 'call'
; and the 'pop ebx'
irq_common_stub:
	pusha
	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
    push esp

	; Call C handler
    cld
	call irq_handler

    pop esp
	pop ebx		; Different than the ISR code , 为什么是ebx 而不是eax ？
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

	popa
	add esp, 8
    iret

; 因为有的中断处理函数会自动压入错误号，而有的不会，这样给我们的清栈造成了麻烦。
; 所以我们在不会压入错误号的中断的处理函数里手动压入0作为占位，这样方便我们在清理的时候不用分类处理。
; 定义两个构造中断处理函数的宏(有的中断有错误代码，有的没有)
; 用于没有错误代码的中断
%macro ISR_NOERRCODE 1      ; define a macro, taking one parameter
    [GLOBAL isr%1]          ; %1 accesses the first parameter.
    isr%1:
        cli                 ; 首先关闭中断
        push byte 0         ; push 无效的中断错误代码
        push byte %1        ; push 中断号
        jmp isr_common_stub
%endmacro

; 用于有错误代码的中断
%macro ISR_ERRCODE 1
    [GLOBAL isr%1]
    isr%1:
        cli
        push byte %1
        jmp isr_common_stub
%endmacro

; 构造中断请求的宏
%macro IRQ 2
    [GLOBAL irq%1]
    irq%1:
        cli
        push byte 0
        push byte %2
        jmp irq_common_stub
%endmacro

; 定义中断处理函数
ISR_NOERRCODE  0    ; 0 #DE 除 0 异常
ISR_NOERRCODE  1    ; 1 #DB 调试异常
ISR_NOERRCODE  2    ; 2 NMI
ISR_NOERRCODE  3    ; 3 BP 断点异常
ISR_NOERRCODE  4    ; 4 #OF 溢出
ISR_NOERRCODE  5    ; 5 #BR 对数组的引用超出边界
ISR_NOERRCODE  6    ; 6 #UD 无效或未定义的操作码
ISR_NOERRCODE  7    ; 7 #NM 设备不可用(无数学协处理器)
ISR_ERRCODE    8    ; 8 #DF 双重故障(有错误代码)
ISR_NOERRCODE  9    ; 9 协处理器跨段操作
ISR_ERRCODE   10    ; 10 #TS 无效TSS(有错误代码)
ISR_ERRCODE   11    ; 11 #NP 段不存在(有错误代码)
ISR_ERRCODE   12    ; 12 #SS 栈错误(有错误代码)
ISR_ERRCODE   13    ; 13 #GP 常规保护(有错误代码)
ISR_ERRCODE   14    ; 14 #PF 页故障(有错误代码)
ISR_NOERRCODE 15    ; 15 CPU 保留
ISR_NOERRCODE 16    ; 16 #MF 浮点处理单元错误
ISR_ERRCODE   17    ; 17 #AC 对齐检查
ISR_NOERRCODE 18    ; 18 #MC 机器检查
ISR_NOERRCODE 19    ; 19 #XM SIMD(单指令多数据)浮点异常

; 20 ~ 31 Intel 保留
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; 定义
IRQ   0,    32  ; 电脑系统计时器
IRQ   1,    33  ; 键盘
IRQ   2,    34  ; 与 IRQ9 相接，MPU-401 MD 使用
IRQ   3,    35  ; 串口设备
IRQ   4,    36  ; 串口设备
IRQ   5,    37  ; 建议声卡使用
IRQ   6,    38  ; 软驱传输控制使用
IRQ   7,    39  ; 打印机传输控制使用
IRQ   8,    40  ; 即时时钟
IRQ   9,    41  ; 与 IRQ2 相接，可设定给其他硬件
IRQ  10,    42  ; 建议网卡使用
IRQ  11,    43  ; 建议 AGP 显卡使用
IRQ  12,    44  ; 接 PS/2 鼠标，也可设定给其他硬件
IRQ  13,    45  ; 协处理器使用
IRQ  14,    46  ; IDE0 传输控制使用
IRQ  15,    47  ; IDE1 传输控制使用
