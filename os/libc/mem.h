#ifndef MEM_H
#define MEM_H

#include "../cpu/type.h"

void memory_copy(uint8_t *source, uint8_t *dest, int nbytes);
void memory_set(uint8_t *dest, uint8_t val, uint32_t len);

/*
 * size         分配内存大小
 * align        1-页对齐，0-不对齐
 * phys_addr    如果不为空，保存分配的内存地址
 */
uint32_t kmalloc_int(size_t size, int align, uint32_t *phys_addr);
uint32_t kmalloc_ap(size_t size, uint32_t *phys_addr);
uint32_t kmalloc(size_t size);

#endif
