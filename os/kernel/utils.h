#ifndef UTIL_H
#define UTIL_H

#include "../cpu/types.h"

void memory_copy(char* source, char* dest, int no_bytes); 
void memeoy_set(u8 *dest, u8 val, u32 len);
void int_to_ascii(int n, char str[]); 
void reverse(char s[]);
int strlen(char s[]);

#endif
