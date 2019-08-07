#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"

int get_cursor();
void set_cursor(int offset);
int print_char(char c, int col, int row, char attr);
int get_screen_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
int handle_scrolling(int offset);

/*******************************************
 * Public functions 
 * *****************************************/

void clear_screen() {
	int row = 0;
	int col = 0;
	for (row = 0; row < MAX_ROWS; row++) {
		for(col=0; col < MAX_COLS; col++) {
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}

	set_cursor(get_screen_offset(0, 0));
}

void print_at(char *message, int col, int row) {
	int offset;
	if(col >= 0 && row >= 0) {
		offset = get_screen_offset(col, row);
	}else {
		offset = get_cursor();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}

	int i = 0;
	while(message[i] != 0) {
		offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void print(char *message) {
	print_at(message, -1, -1);
}

void print_backspace() {
	int offset = get_cursor() - 2;
	int row = get_offset_row(offset);
	int col = get_offset_col(offset);
	print_char(0x08, col, row, WHITE_ON_BLACK);
}

/*******************************************
 * Private functions 
 * *****************************************/
int print_char(char character, int col, int row, char attribute_byte) {
	u8 *vidmem = (u8 *) VIDEO_ADDRESS;
	
	if (!attribute_byte) {
		attribute_byte = WHITE_ON_BLACK;
	}

	if (col >= MAX_COLS || row >= MAX_ROWS) {
		vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
		vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
		return get_screen_offset(col, row);
	}

	int offset;
	if (col >= 0 && row >= 0) {
		offset	= get_screen_offset(col, row);
	} else {
		offset = get_cursor();
	}

	if (character == '\n') {
		int rows = get_offset_row(offset);
		offset = get_screen_offset(0, rows + 1);
	} else if (character == 0x08) { // Backspace
        vidmem[offset] = ' ';
        vidmem[offset+1] = attribute_byte;    
    } else {
		vidmem[offset] = character;
		vidmem[offset+1] = attribute_byte;
		offset += 2;
	}

	offset = handle_scrolling(offset);

	set_cursor(offset);
	return offset;
}

int get_cursor() {
	// 读取光标地址, 询问VGA控制器寄存器0x3d4
	// 14 = 光标高地址, 15 = 光标低地址
	port_byte_out(REG_SCREEN_CTRL, 14);	
	int offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	
	// VGA'单元'由字符及其控制数据组成
	// 例如'黑色背景上的白色'，'白色背景上的红色文字'等
	return offset * 2;
}

void set_cursor(int offset) {
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

int get_screen_offset(int col, int row) { 
	return 2 * (row * MAX_COLS + col); 
}
int get_offset_row(int offset) { 
	return offset / (2 * MAX_COLS); 
}
int get_offset_col(int offset) { 
	return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; 
}

int handle_scrolling(int cursor_offset) {
	if (cursor_offset < MAX_ROWS*MAX_COLS*2) {
		return cursor_offset;
	}
	
	int i;
	for (i=1; i<MAX_ROWS; i++) {
		memory_copy((u8 *)(get_screen_offset(0, i) + VIDEO_ADDRESS),
			    (u8 *)(get_screen_offset(0, i-1) + VIDEO_ADDRESS),
			    MAX_COLS * 2
		);
	}		

	char* last_line = (char *)(get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS);
	for(i=0; i < MAX_COLS*2; i++) {
		last_line[i] = 0;
	}

	cursor_offset -= 2 * MAX_COLS;
	return cursor_offset;
}

