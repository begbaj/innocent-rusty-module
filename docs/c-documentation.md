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

# __user identifier
This keyword is used in the Linux kernel to indicate that this pointer refers to memory in **user space** 
(where regular programs run). 
The kernel has to be careful when accessing user-space memory, as it operates in a different space than 
user programs.

# Dereferencing and memory regions:
When you dereference a pointer, you are asking the system to **go to the memory location stored in the pointer** 
and retrieve the actual data located there. For example, if pathname points to a string, dereferencing it would
mean accessing the characters in the string (e.g., "/home/user/file.txt").

In other words, dereferencing **directly**, without specifing that you're accessing a __user-space__ memory
region, is like going to an address in London (kernel-space) while you wanted to go into that same 
address but in New York (user-space).

The user space and kernel space are separate memory regions (like London and New York are two different cities). 
Programs run in user space (New York), while the kernel operates in its own protected kernel space (London). 
For instance, the `__user` identifier keyword tells the kernel that this pointer points to user space memory
(that the address is refered to New York, not London).

Just like you need to know whether you're going to New York or London, the kernel needs to know it's accessing 
user space memory. This is done with functions like `copy_from_user()` or `strncpy_from_user()`, which safely 
translate or check that the user space memory can be accessed properly before dereferencing it.

# Function Hooking:

A function hook within the kernel refers to a technique where you **modify the behavior of a kernel function** 
by **intercepting its execution**. Essentially, you “hook” into an existing function in the kernel, so that when 
thatfunction is called, **your custom code (or “hook”) is executed either before, after, or instead of 
the original function**.

Function hooks are commonly used for a variety of purposes, such as:
- Monitoring or modifying system calls.
- Adding custom behavior to existing kernel functionality.
- Debugging or tracing.
- Implementing security or performance enhancements.

# Ftrace
Ftrace is a powerful tracing framework built into the Linux kernel. 
It is designed to help developers, system administrators, and kernel engineers analyze the performance 
and behavior of the kernel by tracking and monitoring function calls, context switches, and other kernel 
events. It’s mainly used for debugging and performance tuning at the kernel level, but it can also 
provide insights into the execution of user-space applications when they interact with the kernel.

## Key Features of Ftrace:

- Function Tracing: Ftrace can trace individual function calls made within the kernel. This allows you to monitor the execution path of the kernel and see what functions are being called, their parameters, and their return values.

- Event Tracing: Besides function calls, Ftrace can trace various kernel events, such as:
        Interrupts and context switches.
        Scheduler events, showing which processes are running and for how long.
        I/O events, monitoring when the kernel accesses hardware like storage or network devices.

- Low Overhead: Ftrace is designed to be lightweight. While there is some overhead when tracing, it is minimal, allowing it to be used in production systems for diagnostics without significantly affecting system performance.

- Dynamic Tracing: Ftrace supports dynamic tracing, meaning you can enable and disable tracing of specific functions or events at runtime without needing to reboot the system or recompile the kernel.

# Macro:
A macro in C is a preprocessor directive that allows you to define code that can be reused 
throughout your program. Macros are processed by the C preprocessor before the actual compilation 
takes place. They allow you to define constants, functions, or blocks of code that are 
substituted into the program where the macro is used.

## types:
- **Object-like Macro**: similar to `const`, you use a macro to give **a name** to a **value/expression**.
- **Function-like Macro**: more like **inline functions**. You can define a macro that takes arguments, 
and when the macro is used, the arguments are substituted into the macro definition.

### Object-like example
```C
#define PI 3.14159
float circumference = 2 * PI * radius;
```

### Function-like example
```C
#define SQUARE(x) ((x) * (x))
int result = SQUARE(2+3);  // Becomes: int result = (2+3) * (2+3);
```

## notes:
- **No type checking**: Macros are simply text replacements, so there's no type checking like in functions. 
This can lead to unexpected behavior if you're not careful, especially with function-like macros.

```C
#define SQUARE(x) x * x
int result = SQUARE(2 + 3);  // This expands to 2 + (3 * 2) + 3, while you wanted (2+3) * (2+3)
```
- **Preprocessor Replacement**:
Macros are expanded by the preprocessor before the code is compiled. This means that macros are not functions, 
and they don't have a presence in the compiled code. The preprocessor simply replaces the macro name with 
its definition wherever it is used in the source code.

- **Conditional Macros**: You can use macros conditionally to include or exclude blocks of code, 
often for debugging or cross-platform development.

- **Macro Hygiene**: Since macros are just textual replacements, careless usage can result in bugs. 
For example, if a macro's body isn't properly parenthesized, or if variables inside the macro conflict 
with variables in the surrounding code.


# Designated Initializers
Designated initializers allow you to assign values to specific fields of a structure by name, 
rather than initializing fields in order. This is especially useful when working with large structures 
or when only initializing a subset of fields.

## Struct Designated Initializer

```C
struct syscall_hook {
    const char *name;
    void *function;
    void *original;
};

struct syscall_hook hooks[] = {
    {
        .name = "open",          // Initialize the 'name' field
        .function = my_open_hook, // Initialize the 'function' field
        .original = orig_open,   // Initialize the 'original' field
    }
};
```

The dot `.` allows you to explicitly specify which field of the structure you are initializing. 
This makes the code clearer and less prone to errors, especially when structures have many fields 
or when you don’t want to initialize them in order.

Without the dot (designated initializer), you would need to initialize the fields in the order 
they are declared in the structure, like this:

```C
struct syscall_hook hooks[] = {
    {"open", my_open_hook, orig_open}
};
```

### Nested Structure Initialization
For structures within structures (nested structures), you can use designated initializers for 
inner members as well.

```C
struct inner {
    int x;
    int y;
};

struct outer {
    struct inner in;
    int z;
};

struct outer obj = {
    .in = { .x = 5, .y = 10 },   // Nested initializer for struct 'inner'
    .z = 15
};
```

## Array Designated Initliazier
You can use designated initializers for arrays to initialize specific elements by their index.

```C
int arr[5] = { [0] = 10, [3] = 20 };
```

In this example:
- arr[0] is initialized to 10
- arr[3] is initialized to 20
- The remaining elements are set to 0 by default.

## Partial and Mixing initialization
If you only want to initialize specific fields in a structure or an array, designated initializers 
allow you to do that, leaving the rest of the fields with their default values (usually 0 for numbers).

```C
struct example {
    int a;
    int b;
    int c;
};

struct example obj = {
    .a = 10   // Only 'a' is initialized, 'b' and 'c' are set to 0 by default
};
```

You can also mix designated and positional initializers in the same initialization block, although 
it's not always recommended for readability reasons.

```C
struct example {
    int a;
    int b;
    int c;
};

struct example obj = { 10, .b = 20 }; // 'a' is set to 10, 'b' to 20, 'c' is default 0
```

