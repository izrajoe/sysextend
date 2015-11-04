README
======

"sysextend" is a small kernel module / rootkit that allows you to write syscalls for a running linux kernel.

WARNING!!!
--------------

This library is provided with ***ABSOLUTELY NO GUARANTEE***.  We in fact suggest you ***DON'T USE THIS LIBRARY***. By installing this module, you are ***ACTIVELY COMPROMISING*** the security of your machine.  Use with ***EXTREME CAUTION***.

This module is intended for OS and systems research where recompiling the linux kernel to install a syscall is impossible.

INSTRUCTIONS
-------------

To write your own syscall.

1. Add an entry to the SYSEXTEND enum at the top of *include/sysextend.h*,  e.g. `__NR_dummy`.
2. Write a your syscall in *kernel/sysextend_main.c*, e.g. `sys_dummy`.  The function prototype must match: `unsigned long (foo)(void*)`
3. Also in *kernel/sysextend_main.c*, within `sysextend_init()`, add your function to the extended syscall table using `set_sysextend()`, e.g.	`set_sysextend(__NR_dummy,&sys_dummy);` 


To make your kernel module:

1. cd kernel
2. make

To install your kernel module:

1. cd kernel
2. insmod kobj/sysextended.ko

To uninstall your kernel module:

1. rmmod sysextended.ko

To call your syscall from user space

1. Install the kernel module
2. Include the header *include/sysextend.h* in your user code
3. Use the sysextend function to call the syscall, e.g. `sysextend(__NR_dummy, &args);`


