# PTREGS_SYSCALL_STUBS
refers to a macro or set of macros used in the Linux kernel, specifically in the context of 
system call handling and architecture-specific implementations. These macros are used to define 
how system calls are stubbed, meaning how the kernel sets up the environment to handle a system 
call, particularly dealing with the pt_regs structure (which holds the register state of the 
process that made the system call).

Basically, if `PTREGS_SYSCALL_STUBS` is defined, than the pt_regs struct exists and can be used
to pass sys_call arguments in a more convenient way.

## pt_regs struct:
With kernel version 4.17.0 (since 2016), the arguments that are first stored in registers by the user
are copied into a special struct called **pr_regs**, and then this **is the only thing passed to the syscall**.
Syscall is then responsible for **pullings the arguments it needs** out of this struct.
```c
    struct pt_regs {
        unsigned long bx;
        unsigned long cx;
        unsigned long dx;
        unsigned long si;
        unsigned long di;
        /* redacted for clarity */
    };
```
This structure holds the CPU register states, which are used to pass arguments during a system call.
The pt_regs structure provides a convenient way for the kernel to access these arguments, especially 
on architectures where system call arguments are passed through registers.
