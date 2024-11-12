# Recursive loops handling
A recursive loop occurs when a function continuosly calls itself in an infinite loop.

While developing kernel modules that have function hooked to syscalls, this can easly occur because of ftrace that adds a hookable piece of code on top of every kernel function.

__"The ftrace infrastructure has some protections agains recursions and RCU but one must still be very careful how they use the callbacks."__

# xcellerator method of handling
xcellerator uses this piece of code:

```C
    *((unsigned long*) hook->original) = hook->address + MCOUNT_INSN_SIZE;
```

to jump over the mcount, which is the **function profiler**, which is placed at the start of **every kernel function**. 
This is produced by the `-pg` switch in gcc.

He does that so it leave @rip register free for use, since the built-in recursion handling of ftrace uses that register for saving return registers.
So **we have to disable** the protection ftrace provides setting:
- FTRACE_OPS_FL_IP_MODIFY 
- FTRACE_OPS_FL_SAVE_REGS
- FTRACE_OPS_FL_RECURSION_SAFE.


# resources:
- https://www.kernel.org/doc/html/v4.16/trace/ftrace-uses.html?highlight=fentry
