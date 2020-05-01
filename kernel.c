// 如果不声明入口函数，只会运行 some_func，因为 some_func 第一个语句并返回了
void some_func() {
	char *video_memory = (char *)0xb8002;  // 屏幕地址
	*video_memory = '2'; 	      // 左上角显示字符
}

void main() {
	char *video_memory = (char *)0xb8000;  // 屏幕地址
	*video_memory = '1'; 	      // 左上角显示字符

	some_func();
}
