#include "idt.h"

void set_idt_gate(int n, uint32_t handler) {
	idt[n].base_lo = low_16(handler);			// 中断处理函数地址低 16 位
	idt[n].sel = KERNEL_CS;						// 目标代码段描述符选择子
	idt[n].always0 = 0;
	idt[n].flags = 0x8E;  						// 1000 1110b
	idt[n].base_hi = high_16(handler);			// 中断处理函数地址高 16 位
}

void set_idt() {
	idt_reg.base = (uint32_t) &idt;
	idt_reg.limit = (sizeof(idt_gate_t) * IDT_ENTRIES) - 1;

    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    asm volatile("lidtl (%0)" : : "r" (&idt_reg));
}
