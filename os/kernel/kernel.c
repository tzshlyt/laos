#include "../drivers/ports.h"
void some_func() {
	char *video_memory = (char *)0xb8002;  // 屏幕地址
	*video_memory = 'O'; 	      // 左上角显示字符
	
}
int main() {
	//char *video_memory = (char *)0xb8000;  // 屏幕地址
	//*video_memory = 'X'; 	      // 左上角显示字符

	//some_func();

	// 读取光标地址, 询问VGA控制器寄存器0x3d4
	// 14 = 光标高地址, 15 = 光标低地址
	port_byte_out(0x3d4, 14);

	int position = port_byte_in(0x3d5);
	position = position << 8;

	port_byte_out(0x3d4, 15);
	position += port_byte_in(0x3d5);

	// VGA'单元'由字符及其控制数据组成
	// 例如'黑色背景上的白色'，'白色背景上的红色文字'等
	int offset_from_vga = position * 2;

	char *vga = (char *)0xb8000;
	vga[offset_from_vga] = 'X';
	vga[offset_from_vga+1] = 0x0f;
}
