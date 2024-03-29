# NextOS

[![Gitpod ready-to-code](https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod)](https://gitpod.io/#https://github.com/Glowman554/NextOS)
![lines_of_code](https://tokei.rs/b1/github/Glowman554/NextOS)
![Build](https://github.com/Glowman554/NextOS/workflows/C/C++%20CI/badge.svg)
![Automatic Release](https://github.com/Glowman554/NextOS/workflows/Automatic%20Release/badge.svg)

A simple os written in c and c++.  
It's a more advanced and more stable kernel than my first kernel [GKernel](https://github.com/Glowman554/gkernel)  

## Featurs

- App loading with Grub
- Keyboard support
- Mouse support
- PCI support 
- Multitasking support
- Driver Abstraction
- VGA Mode
- Serial COM1 Debug Log
- Initrd
- ATA Driver

## Building
You need to install `mkisofs gcc g++ as ld nasm make`.  
To create an ISO file you need to enter `make iso`.  

## Demo

<a href="https://glowman554.gq/NextOS/?profile=custom&cdrom.url=https://glowman554.gq/NextOS/cdrom.iso">Click here to run demo!</a>  

## Syscalls

| Name | eax | ebx | ecx | edx |
| - | - | - | - | - |
| SYSCALL_PUTC | 0 | char c | - | - |
| SYSCALL_PUTS | 1 | char* s | - | - |
| SYSCALL_PUTN | 2 | int number | int base | -
| SYSCALL_CLRSCR | 3| - | - | - | 
| SYSCALL_SETCOLOR | 4 | uint32_t color | - | - |
| SYSCALL_KVERSION | 5 | int ver (return) | - | -
| SYSCALL_KVENDOR | 6 | char* ven (return) | - | - |
| SYSCALL_EXEC | 7 | char* file | - | -
| SYSCALL_INIT_TASK | 8 | void* entry | - | - |
| SYSCALL_TASK_EXIT | 9 | int errorcode | - | - |
| SYSCALL_REBOOT | 10 | - | - | - |
| SYSCALL_GET_TICK | 12 | uint32_t tick (return) | - | - |
| SYSCALL_RESET_TICK | 13 | - | - | - |
| SYSCALL_MULTIBOOT | 14 | multiboot_info *mb_info | - | - |
| SYSCALL_VGA_MODE | 15 | - | - | - |
| SYSCALL_VGA_SETPIXEL | 16 | int x | int y | uint32_t color |
| SYSCALL_VGA_SETCOLOR | 17 | uint32_t fgcolor | uint32_t bgcolor | - |
| SYSCALL_LOAD_INITRD | 18 | char* file | - | - |
| SYSCALL_FSROOT_READDIR | 19 | int index | struct dirent *node (return) | - |
| SYSCALL_FSROOT_FINDDIR | 20 | char* dir | fs_node_t *fsnode (return) | - |
| SYSCAlL_FSROOT_READ | 21 | uint32_t offset | uint32_t size | fs_node_t *fsnode |
| SYSCALL_GET_BUFFER | 22 | uint8_t* buf (return) | - | - |
| SYSCALL_INIT_DRIVER | 23 | char* name | driver_handler_ptr driver_handler | int id (return) |
| SYSCALL_CALL_DRIVER | 24 | int id | void* data | int data (return) |
| SYSCALL_FIND_DRIVER | 25 | char* name | int id (return) | - |
| SYSCALL_DRAW_CHAR | 26 | char c | int x | int y |
| SYSCALL_SETX | 27 | int x | - | - |
| SYSCALL_SETY | 28 | int y | - | - |
| SYSCALL_GETX | 29 | int x (return) | - | - |
| SYSCALL_GETY | 30 | int y (return) | - | - |
| SYSCALL_SET_KB_HANDLER | 31 | kb_handler handler | - | - |
| SYSCALL_SET_MOUSE_HANDLER | 32 | mouse_move_handler h1 | mouse_button_handler h2 | - |
| SYSCALL_GETPIXEL | 33 | int x | int y | char c (return) |

## Copyright

See LICENSE
