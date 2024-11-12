# asmlinkage
is a keyword used in the Linux kernel that **tells the compiler how to handle function calls**. 
It helps the system **know that this function is a system call**, which is a way for programs to 
ask the operating system (like Linux) to do things, such as reading from a file.

In the context of the Linux kernel, asmlinkage is a keyword that tells the compiler to pass all 
the function arguments through the CPU's registers, not the normal way that C functions pass 
arguments (which can involve the stack, a reserved memory area in RAM, usually in user-space).

This is important because system calls are triggered in a special way, often using a software 
interrupt or special instruction, and the CPU handles them differently from normal function 
calls.

