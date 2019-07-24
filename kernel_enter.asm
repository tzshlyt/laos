; 确保直接跳到内核入口函数
[bits 32]
[extern main]
call main
jmp $
