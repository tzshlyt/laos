#include "idt.h"
#include "../kernel/utils.h"

void set_idt_gate(int n, u32 handler) {
	idt[n].base_lo = low_16(handler);
	idt[n].sel = KERNEL_CS;
	idt[n].always0 = 0;
	idt[n].flags = 0x8E;  // 1000 1110b
	idt[n].base_hi = high_16(handler);
}

void set_idt() {
	idt_reg.base = (u32) &idt;
	idt_reg.limit = (sizeof(idt_gate_t) * 256) - 1;
	
       	/* Don't make the mistake of loading &idt -- always load &idt_reg */
   	__asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
