void some_func() {
	char *video_memory = (char *)0xb8002;  // 屏幕地址
	*video_memory = 'O'; 	      // 左上角显示字符
	
}
void main() {
	char *video_memory = (char *)0xb8000;  // 屏幕地址
	*video_memory = 'X'; 	      // 左上角显示字符

	some_func();
}
