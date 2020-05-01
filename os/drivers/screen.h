#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS 0xb8000       // 屏幕映射内存地址, 固定地址
#define MAX_ROWS 25                 // 最大行, 设备决定
#define MAX_COLS 80                 // 最大列

#define WHITE_ON_BLACK 0x0f         // 字符属性，背景色
#define RED_ON_WHITE 0xf4

/* 屏幕设备 I/O ports */
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

void clear_screen();
void kprint_at(char *message, int col, int row);
void kprint(char *message);
void kprint_backspace();

#endif
