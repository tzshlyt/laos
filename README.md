# laos

#### Mem management

​       0 Higher half loader [✔]
​       1 physical memory management []
​       2 paging(virtual memory management) []
​       3 kmalloc []

#### Filesystem

​       0 PCI Enumerate,R/W []
​       1 ATA/DMA Driver []
​       2 Ext2 Filesystem []
​       3 Virtual Filesystem []

#### Graphical Interface

​       0 vesa driver, 1024 * 768 color(32-bit color)，framebuffer mode []
​       1 Milestone(try loading and showing a wallpaper from hardisk!) []
​       2 Windows compositor, support fonts, buttons,and etc []
​       3 Write a shell []
​       4 Write a file browser []

#### Miscellaneous

​       1 Usermode []
​       1 Multi-tasking/Simple scheduler []
​       2 Executable loader(elf) []
​       3 spinlock, mutex, and other pthread primitives []
​       4 Add more syscalls, like fork/exec, open, read, etc.. []
​       5 Port newlib []
​       6 Write or port some baisc utilities such asenv, rm, cp, mkdir, reboot []
​       7 Real time clock []
​       9 stadard input/output stuff []

#### Network protocols

​       0 Send/Recv raw packets []
​       1 IP []
​       2 UDP []
​       3 ARP []
​       4 DHCP []
​       5 TCP []
​       6 HTTP []