#include "timer.h"
#include "../drivers/screen.h"
#include "isr.h"
#include "../libc/function.h"
#include "ports.h"

uint32_t tick = 0;

static void timer_callback(registers_t *regs) {
	tick++;
	
    /*
    print("Tick: ");
	char tick_ascii[256];
	int_to_ascii(tick, tick_ascii);
	print(tick_ascii);
	print("\n");
    */
    UNUSED(regs);
}

// https://wiki.osdev.org/Programmable_Interval_Timer
void init_timer(uint32_t freq) {
	// PIT
	register_interrupt_handler(IRQ0, timer_callback);
		
	// hardware colock at 1193180 Hz
	// COUNT = input hz / frequency
	// 1193180 / 100	; 100hz, or 10 milliseconds
 	uint32_t divisor = 1193180 / freq;
	uint8_t low  = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

	// Send the command
	// 0bit -> 0, 0 = 16-bit binary, 1 = four-digit BCD
	// 1-3bit -> 011,  Mode 3 (square wave generator)   
	// 4-5bit -> 11, 11 = Access mode: lobyte/hibyte
	port_byte_out(0x43, 0x36); //0x36 --> 110110b,
	port_byte_out(0x40, low);
	port_byte_out(0x40, high);
}
