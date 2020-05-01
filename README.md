# laos

- Boot from scratch, without GRUB - [✔]
- Enter 32-bit mode - [✔]
- Jump from Assembly to C - [✔]
- Interrupt handling - [✔]
- Screen output and keyboard input - [✔]
- A tiny, basic `libc` which grows to suit our needs - [✔]
- Memory management
- Write a filesystem to store files
- Create a very simple shell
- User mode
- Maybe we will write a simple text editor
- Multiple processes and scheduling
- A BASIC interpreter, like in the 70s!
- A GUI
- Networking

#### Mem management

0. Higher half loader [✔]
1. physical memory management []
2. paging(virtual memory management) []
3. kmalloc []

#### Filesystem

0. PCI Enumerate,R/W []
1. ATA/DMA Driver []
2. Ext2 Filesystem []
3. Virtual Filesystem []

#### Graphical Interface

0. vesa driver, 1024 * 768 color(32-bit color)，framebuffer mode []
1. Milestone(try loading and showing a wallpaper from hardisk!) []
2. Windows compositor, support fonts, buttons,and etc []
3. Write a shell []
4. Write a file browser []

#### Miscellaneous

0. Usermode []
1. Multi-tasking/Simple scheduler []
2. Executable loader(elf) []
3. spinlock, mutex, and other pthread primitives []
4. Add more syscalls, like fork/exec, open, read, etc.. []
5. Port newlib []
6. Write or port some baisc utilities such asenv, rm, cp, mkdir, reboot []
7. Real time clock []
9. stadard input/output stuff []

#### Network protocols

0. Send/Recv raw packets []
1. IP []
2. UDP []
3. ARP []
4. DHCP []
5. TCP []
6. HTTP []

##### 如何运行

```shell
gcc -ffreestanding -c kernel.c -o kernel.o
ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat binary
nasm boot_sect.asm -f bin -o boot_sect.bin
cat boot_sect.bin kernel.bin > os-image.bin
qemu-system-i386 -fda os-image.bin

或者
make clean
make run
```

