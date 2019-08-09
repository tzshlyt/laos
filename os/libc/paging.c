#include "paging.h"
#include "../libc/string.h"

// A bitset of frames - used or free.
uint32_t *frames;
uint32_t nframes;

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
        if( frames[i] != 0xFFFFFFFF) {  // nothing free, exit early.
            for(j = 0; j < 32; j++) {   // at least one bit is free here.
                uint32_t toTest = 0x1 << j;
                if ( !(frames[i] & toTest) ) {
                    return i * 32 + j;
                }
            }
        }
    }
    return 0xFFFFF;
}

// Page now doesn't have a frame.
void create_frame(page_t *page, uint32_t is_kernel, uint32_t is_writeable) {
    if(page->frame != 0) {
        return;
    }

    uint32_t idx = first_frame();  // idx is now the index of the first free frame.
    if (idx == 0xFFFFF) {   // 这里idx 最大不应该是 4G / 4K  = 1M 吗
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
void free_frame(page_t *page) {
    uint32_t frame;
    if (!(frame = page->frame)) {
        return;
    }
    clear_frame(frame * 0x1000);
    page->frame = 0x0;      // Page now doesn't have a frame. 
}

void initialise_paging() {
    uint32_t mem_end_page = 0x1000000;
}
