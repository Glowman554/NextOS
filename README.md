# NextOS

A simple os written in c and c++.  
It's a more advanced and more stable kernel than my first kernel [GKernel](https://github.com/Glowman554/gkernel)  

## Featurs

- App loading with Grub
- Keyboard support
- PCI support 
- Multitasking support
- Driver Abstraction

## Building
You need to install `genisoimage gcc g++ as ld`.  
To create an ISO file you need to enter `make iso`.  

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
| SYSCALL_GETCHAR | 11 | char c (return) | - | - |
| SYSCALL_GET_TICK | 12 | uint32_t tick (return) | - | - |
| SYSCALL_RESET_TICK | 13 | - | - | - |
| SYSCALL_MULTIBOOT | 14 | multiboot_info *mb_info | - | - |

## Copyright

See LICENSE
