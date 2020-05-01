; 确保直接跳到内核入口函数
; 写一个非常简单的汇编程序附加到内核代码开始位置
; 链接器的所做的工作
; 因为此代码不能单独存在, 依赖其它标签, 所以编译成文件对象 .o 而不是二进制文件 .bin, 并保留标签的源信息
;
; $nasm kernel_entry.asm -f elf -o kernel_entry.o
;
; The option -f elf tells the assembler to output an object ﬁle of the particular format Executable and Linking Format (ELF),
; which is the default format output by out C compiler

; 链接文件
; ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary
;
; 链接器依赖文件传入的顺序，所以 kernel_entry.o 文件在 kernel.o 前面

; 连接文件
; cat boot_sect.bin kernel.bin > os-image
;

; 运行
; qemu-system-i386 -fda os-image

[bits 32]
[extern main]       ; 声明 main 函数, 因为 main 函数在另外一个文件中，也因为链接后此代码在内核代码顶部，需要声明调用的函数。定义调用地址，必须和 kernel.c 文件 main 函数名字相同
call main           ; 调用 main, 编译器知道 main 函数的地址
jmp $
