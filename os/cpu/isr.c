#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "timer.h"
#include "ports.h"
#include "../libc/string.h"

isr_t interrupt_handlers[256];  // ISR 中断函数


/* Can't do this with a loop because we need the address
 * of the function names */
void isr_install() {
	set_idt_gate(0, (uint32_t)isr0);
	set_idt_gate(1, (uint32_t)isr1);
	set_idt_gate(2, (uint32_t)isr2);
	set_idt_gate(3, (uint32_t)isr3);
	set_idt_gate(4, (uint32_t)isr4);
	set_idt_gate(5, (uint32_t)isr5);
	set_idt_gate(6, (uint32_t)isr6);
	set_idt_gate(7, (uint32_t)isr7);
	set_idt_gate(8, (uint32_t)isr8);
	set_idt_gate(9, (uint32_t)isr9);
	set_idt_gate(10, (uint32_t)isr10);
	set_idt_gate(11, (uint32_t)isr11);
	set_idt_gate(12, (uint32_t)isr12);
	set_idt_gate(13, (uint32_t)isr13);
	set_idt_gate(14, (uint32_t)isr14);
	set_idt_gate(15, (uint32_t)isr15);
	set_idt_gate(16, (uint32_t)isr16);
	set_idt_gate(17, (uint32_t)isr17);
	set_idt_gate(18, (uint32_t)isr18);
	set_idt_gate(19, (uint32_t)isr19);
	set_idt_gate(20, (uint32_t)isr20);
	set_idt_gate(21, (uint32_t)isr21);
	set_idt_gate(22, (uint32_t)isr22);
	set_idt_gate(23, (uint32_t)isr23);
	set_idt_gate(24, (uint32_t)isr24);
	set_idt_gate(25, (uint32_t)isr25);
	set_idt_gate(26, (uint32_t)isr26);
	set_idt_gate(27, (uint32_t)isr27);
	set_idt_gate(28, (uint32_t)isr28);
	set_idt_gate(29, (uint32_t)isr29);
	set_idt_gate(30, (uint32_t)isr30);
	set_idt_gate(31, (uint32_t)isr31);

	// Remap the PIC (programmable interrupt controller)
	// https://wiki.osdev.org/PIC
	// 8259A PIC每一片可以管理8个中断源，显然一般情况下设备数量会超过这个值。
	// IBM的设计方案是使用8259A PIC 的级联功能，使用两片级联（分为主、从片）的方式来管理硬件中断
	// 因为计算机启动的时候，默认的中断对应如下，所以需要重新映射
	// IRQ 0..7 - INT 0x8..0xF
	// IRQ 8..15 - INT 0x70..0x77
	// 主片端口 command: 0x20 data: 0x21
    // 从片端口 command: 0xA0 data: 0xA1

	// 初始化主片、从片
	port_byte_out(0x20, 0x11); // 0x10 Initialization - required!, 0x01 ICW4 (not) needed
	port_byte_out(0xA0, 0x11);

	// 设置主片 IRQ 从 0x20 (32) 号中断开始
	port_byte_out(0x21, 0x20); // ICW2: Master PIC vector offset
	// 设置从片 IRQ 从 0x28(40) 号中断开始
	port_byte_out(0xA0, 0x28); // ICW2: Slave PIC vector offset
	// 设置主片 IR2 引脚连接从片
	port_byte_out(0x21, 0x04); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	// 告诉从片输出引脚和主片 IR2 号相连
	port_byte_out(0xA1, 0x02); // ICW3: tell Slave PIC its cascade identity (0000 0010)
	// 设置主片和从片按照 8086 的方式工作
	port_byte_out(0x21, 0x01); // 8086/88 (MCS-80/85) mode
	port_byte_out(0xA1, 0x01); // 8086/88 (MCS-80/85) mode
	// 设置主从片允许中断
	port_byte_out(0x21, 0x0);  // restore saved masks
	port_byte_out(0xA1, 0x0);

	// Install the IRQs
	set_idt_gate(32, (uint32_t)irq0);
	set_idt_gate(33, (uint32_t)irq1);
	set_idt_gate(34, (uint32_t)irq2);
	set_idt_gate(35, (uint32_t)irq3);
	set_idt_gate(36, (uint32_t)irq4);
	set_idt_gate(37, (uint32_t)irq5);
	set_idt_gate(38, (uint32_t)irq6);
	set_idt_gate(39, (uint32_t)irq7);
	set_idt_gate(40, (uint32_t)irq8);
	set_idt_gate(41, (uint32_t)irq9);
	set_idt_gate(42, (uint32_t)irq10);
	set_idt_gate(43, (uint32_t)irq11);
	set_idt_gate(44, (uint32_t)irq12);
	set_idt_gate(45, (uint32_t)irq13);
	set_idt_gate(46, (uint32_t)irq14);
	set_idt_gate(47, (uint32_t)irq15);

	set_idt(); // Load with ASM
}

/*
 *	To print the message which defines every exception
 *  前 32 个 中断
*/
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t *r) {
    kprint("received interrupt: ");
    char s[3];
    int_to_ascii(r->int_no, s);
    kprint(s);
    kprint("\n");
    kprint(exception_messages[r->int_no]);
    kprint("\n");
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
	interrupt_handlers[n] = handler;
}

void irq_handler(registers_t *r) {
	// After every interrupt we need to send an EOI to the PICs
	// or they will not send another interrupt again
	// 0x20 End-of-interrupt command code
	// 发送中断结束信号给 PICs
    // 按照我们的设置，从 32 号中断起为用户自定义中断
    // 因为单片的 Intel 8259A 芯片只能处理 8 级中断
    // 故大于等于 40 的中断号是由从片处理的
	if(r->int_no >= 40) {  // if the IRQ came from the Slave PIC
		// 发送重设信号给从片
		port_byte_out(0xA0, 0x20); // slave
	}
	// 发送重设信号给主片
	port_byte_out(0x20, 0x20); // master

	/* Handle the interrupt in a more modular way */
	if (interrupt_handlers[r->int_no] != 0) {
		isr_t handler = interrupt_handlers[r->int_no];
		handler(r);
	}
}

void irq_install() {
    // Enable interruptions
    asm volatile("sti");
    // IRQ0: timer
    init_timer(50);
    // IRQ1: keyboard
    init_keyboard();
}
