#include "paging.h"
#include "string.h"
#include "mem.h"
#include "../drivers/screen.h"

// 内核page
page_directory_t *kernel_directory = 0;

// 当前page
page_directory_t *current_directory = 0;

// A bitset of frames - used or free.
uint32_t *frames;       //frame bitmap
uint32_t nframes;       //frame个数

// Defined in mem.c
extern uint32_t free_mem_addr;

// A bitset of frames - used or free.
#define INDEX_FROM_BIT(a) (a/32)
#define OFFSET_FROM_BIT(a) (a%32)

// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;   // 4K
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

// Static function to set a bit in the frames bitset
static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;   // 4K
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
static uint32_t test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;   // 4K
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    return (frames[idx] &  (0x1 << off));
}

// Static function to find the first free frame.
static uint32_t first_frame() {
    uint32_t i, j;
    for(i = 0; i < INDEX_FROM_BIT(nframes); i++) {
        if( frames[i] != 0xFFFFFFFF ) {  // nothing free, exit early.
            for(j = 0; j < 32; j++) {   // at least one bit is free here.
                uint32_t toTest = 0x1 << j;
                if ( !(frames[i] & toTest) ) {
                    return i * 32 + j;
                }
            }
        }
    }
    return (uint32_t) - 1;
}

// Page now doesn't have a frame.
// 1. 置位frames bitmap
// 2. 设置page_t
 void create_frame(page_t *page, uint32_t is_kernel, uint32_t is_writeable) {
    if(page->frame != 0) {
        return;
    }

    uint32_t idx = first_frame();  // idx is now the index of the first free frame.
    if (idx == (uint32_t) - 1) {   // 没有找到可用的frame，即first_frame 没有返回
        //print("No free frames!\n");
        return;
    }

    set_frame(idx * 0x1000);
    page->present = 1;
    page->rw = is_writeable ? 1 : 0;
    page->user = is_kernel ? 0 : 1;
    page->frame = idx;
}

// Function to deallocate a frame.
// 1. 清0 frames bitmap
// 2. 设置page_t->frame 为0
void free_frame(page_t *page) {
    uint32_t frame;
    if (!(frame = page->frame)) {
        return;
    }
    clear_frame(frame * 0x1000);
    page->frame = 0x0;      // Page now doesn't have a frame.
}

void initialise_paging() {
    // 物理内存大小，此时设置为16M，为什么16M
    uint32_t mem_end_page = 0x1000000;

    nframes = mem_end_page / 0x1000;    // 多少页, 16M/4K = 4K页
    frames = (uint32_t *)kmalloc(INDEX_FROM_BIT(nframes));      // 分配内存给frames bitmap
    memory_set((uint8_t *)frames, 0, INDEX_FROM_BIT(nframes));  // TODO: 这里应该是 nframes / 8 ？？

    kernel_directory = (page_directory_t *)kmalloc(sizeof(page_directory_t));
    current_directory = kernel_directory;

    uint32_t i = 0;
    while( i < free_mem_addr) {
        create_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    register_interrupt_handler(14, page_fault);

    switch_page_directory(kernel_directory);
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir) {
    address /= 0x1000;                       // 地址转换为分页
    uint32_t table_idx = address / 1024;
    if(dir->tables[table_idx]) {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make) {
        uint32_t tmp;
        dir->tables[table_idx] = (page_table_t *)kmalloc_ap(sizeof(page_table_t), &tmp);
        dir->tablesPhysical[table_idx] = tmp | 0x7;     // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else {
        return 0;
    }
}

void switch_page_directory(page_directory_t *dir) {
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void page_fault(registers_t *regs) {
    // 故障地址保持在cr2中
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    int present = !(regs->err_code & 0x1);
    int rw = regs->err_code & 0x2;
    int us = regs->err_code & 0x4;
    int reserved = regs->err_code & 0x8;
    int id = regs->err_code & 0x10;

    kprint("Page fault! ( ");
    if (present) { kprint("present "); }
    if (rw) { kprint("read-only "); }
    if (us) { kprint("user-mode "); }
    if (reserved) { kprint("reserved "); }
    if (id) { kprint("id "); }
    kprint(") at 0x");
    kprint("\n");
}
