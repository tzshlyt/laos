// 函数调用实验
int callee_function(int my_arg) {
	return my_arg;
}

void caller_function() {
	callee_function(0xdede );
}

