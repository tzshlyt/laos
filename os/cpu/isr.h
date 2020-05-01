//Interrupt Service Routines (ISR) are located (one per interrupt vector).

#ifndef ISR_H
#define ISR_H

#include "type.h"

/* ISR reserved for CPU exceptions, 前 32 个中断*/
// 声明中断处理函数 0 ~ 19 属于 CPU 的异常中断
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
// 20 ~ 31 Intel 保留
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
// 32 ~ 255 用户自定义异常

// IRQ definitions
// interrupt requests (IRQs)
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// 定义IRQ
#define IRQ0 32      // 电脑系统计时器
#define IRQ1 33      // 键盘
#define IRQ2 34      // 与 IRQ9 相接，MPU-401 MD 使用
#define IRQ3 35      // 串口设备
#define IRQ4 36      // 串口设备
#define IRQ5 37      // 建议声卡使用
#define IRQ6 38      // 软驱传输控制使用
#define IRQ7 39      // 软驱传输控制使用
#define IRQ8 40      // 即时时钟
#define IRQ9 41      // 与 IRQ2 相接，可设定给其他硬件
#define IRQ10 42     // 建议网卡使用
#define IRQ11 43     // 建议 AGP 显卡使用
#define IRQ12 44     // 接 PS/2 鼠标，也可设定给其他硬件
#define IRQ13 45     // 协处理器使用
#define IRQ14 46     // IDE0 传输控制使用
#define IRQ15 47     // IDE1 传输控制使用

/* Struct which aggregates many registers.
 * It matches exactly the pushes on interrupt.asm. From the bottom:
 * - Pushed by the processor automatically
 * - `push byte`s on the isr-specific code: error code, then int number
 * - All the registers by pusha
 * - `push eax` whose lower 16-bits contain DS
 * CPU保护的寄存器和剩余需要保护的寄存器一起定义的结构体
 */
// 编译器支持
// typedef struct {
//    uint32_t ds; /* Data segment selector 用于保存用户的数据段描述符 */
//    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax; /* Pushed by pusha. */
//    uint32_t int_no, err_code; /* Interrupt number and error code (if applicable), 中断号和错误代码(有中断错误代码的中断会由CPU压入) */
//    uint32_t eip, cs, eflags, esp, ss; /* Pushed by the processor automatically */
// } registers_t;

typedef struct {
   uint32_t ds; /* Data segment selector */
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
   uint32_t int_no, err_code; /* Interrupt number and error code (if applicable) */
   uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
} registers_t;

// 初始化
void isr_install();

// 调用中断处理函数
void isr_handler(registers_t *r);

// IRQ 初始化
void irq_install();

// 定义中断处理函数指针
typedef void (*isr_t)(registers_t *);

// 注册中断函数
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif
