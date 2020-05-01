#include "kernel.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"
#include "../libc/paging.h"


void some_func() {
	char *video_memory = (char *)0xb8000;  // 屏幕地址
	*video_memory = 'X'; 	      		   // 左上角显示字符
}

void some_func1() {
	char *video_memory = (char *)0xb8002;  // 屏幕地址
	*video_memory = 'O'; 	      // 左上角显示字符
}

void some_func2() {
	//读取光标地址, 询问VGA控制器寄存器0x3d4
	//14 = 光标高地址, 15 = 光标低地址
	port_byte_out(0x3d4, 14);

	int position = port_byte_in(0x3d5);
	position = position << 8;

	port_byte_out(0x3d4, 15);
	position += port_byte_in(0x3d5);

	//VGA'单元'由字符及其控制数据组成
	//例如'黑色背景上的白色'，'白色背景上的红色文字'等
	int offset_from_vga = position * 2;

	char *vga = (char *)0xb8000;
	vga[offset_from_vga] = 'X';
	vga[offset_from_vga+1] = 0x0f;
}

// 屏幕实验
void test_screen() {
	clear_screen();
	kprint_at("X", 0, 0);
	kprint_at("X", 1, 1);
	kprint_at("E", MAX_COLS - 1, MAX_ROWS - 1);
	kprint_at("hello world!", 0, 2);
	kprint_at("This text spans multiple lines", 75, 10);
	kprint_at("?", -1, -1);
	kprint_at("There is a line\nbreak", 0, 20);
	kprint("!!There is a line\nbreak");
	kprint_at("What happens when we run out of space?", 45, 24);
}

// 屏幕滚动实验
void test_scroll() {
	clear_screen();
	int i = 0;
	for (i = 0; i < 24; i++) {
		char str[255];
		int_to_ascii(i, str);
		kprint_at(str, 0, i);
	}
	kprint_at("This text forces the kernel to scroll. Row 0 will disappear. ", 60, 24);
	kprint("And with this text, the kernel will scroll again, and row 1 will disappear too!");
	kprint("=");
}

void test_interrupt() {
	isr_install();
	asm volatile("int $2");
	asm volatile("int $3");
}

void test_timer() {
	isr_install();

	asm volatile("sti");
	init_timer(100); // 100Hz 10ms, 但是显示效果好像不对
}

void test_keyboard() {
	isr_install();

	asm volatile("sti");
	init_keyboard();
}

void test_irq() {
	isr_install();
	irq_install();

    asm volatile("int $2");
    asm volatile("int $3");

    kprint("Type something, it will go through the kernel\n"
        "Type END to halt the CPU\n> ");
}

void test_paging() {
    isr_install();
    irq_install();

    // initialise_paging();
    kprint("Hello, paging world!");
}

void kernel_main() {
	// test_screen();
	// test_scroll();
	// test_interrupt();
	// test_irq();
	// test_timer();
	// test_keyboard();

	test_paging();
}

void user_input(char *input) {
	if(strcmp(input, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (strcmp(input, "PAGE") == 0) {
        uint32_t phys_addr;
        uint32_t page = kmalloc_int(1000, 1, &phys_addr);
        char page_str[16] = "";
        hx_to_ascii(page, page_str);
        char phys_str[16] = "";
        hx_to_ascii(phys_addr, phys_str);

		kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n");

		page = kmalloc_int(1, 1, &phys_addr);
		char page_str1[16] = "";
        hx_to_ascii(page, page_str1);
        char phys_str1[16] = "";
        hx_to_ascii(phys_addr, phys_str1);

		kprint("Page: ");
        kprint(page_str1);
        kprint(", physical address: ");
        kprint(phys_str1);
        kprint("\n");
    }

    kprint("You said: ");
	kprint(input);
	kprint("\n> ");
}
