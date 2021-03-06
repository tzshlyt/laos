/*
	Interrupt Service Routines (ISR) are located (one per interrupt vector).
	It is similar to the Global Descriptor Table in structure.
*/

#ifndef IDT_H
#define IDT_H

#include "type.h"

// Segment selectors
// GDT 中设置的 code segment 偏移
#define KERNEL_CS 0x08

// Defines an IDT entry
typedef struct {
	uint16_t base_lo;		// The lower 16 bits of the address to jump to when this interrupt fires.
	uint16_t sel;			// Kernel segment selector.
	uint8_t  always0;		// This must always be zero.
	/* First byte
	 * Bit 7: "Interrupt is present"
	 * Bits 6-5: Privilege level of caller (0=kernel..3=user)
	 * Bit 4: Set to 0 for interrupt gates
	 * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate"
	 */
	uint8_t  flags;  // 1000 1110b, 0-4 always 00110
	uint16_t base_hi;		// The upper 16 bits of the address to jump to.
} __attribute__((packed)) idt_gate_t;
// __attribute__((packed)) 告诉编译器取消结构在编译过程中的优化对齐

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
// Assembly instruction 'lidt' will read it
typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_register_t;

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, uint32_t handler);
void set_idt();

#endif
