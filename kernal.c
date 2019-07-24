void main() {
	char *video_memory = (char *)0xb8000;  // 屏幕地址
	*video_memory = 'X'; 	      // 左上角显示字符
}
