__Tutorial by xcellerator (with notes by begbaj)__

# Our First Syscall Hook
With this module, we're going to create a very simple hook for `sys_mkdir` that prints out the name
of the directory being created to the kernel buffer.

Afterwards we'll worry about actually getting this hook used **instead of the real sys_mkdir**.

First, notice how we have 2 almost identical functions separated by an **if/else preprocessor condition**.
The condition checks if `PTREGS_SYSCALL_STUBS` exists in the kernel, and if it does, than the `pt_regs` struct
is used. If it is not defined, the function will give the full declaration using the actual names of the 
arguments.

```C
    char __user *pathname = (char *)regs->di;
```

Notice in the first version of the hook (where we use pt_regs), we also have to include the line above
in order to **pull the pathname argument** out of the regs struct.

Basically, __pathname__ points the value of reg __di__ and the newly created pointer is defined in user-space.

For example:
- If a user program makes a system call like open("/home/user/file.txt"), the string "/home/user/file.txt" is passed to the kernel as an argument.
- This string (or its memory address) would be placed in the di register by the CPU.
- Inside the kernel, the system call handler (e.g., sys_open) would extract this argument from regs->di using this line, and the kernel would then interpret this pointer as the file path the user wants to open.

The other important thing to notice is the use of the `strncpy_from_user()` function. 
The presence of the `__user` **identifier** for the pathname argument means that it points to a location 
in **userspace** which isn’t necessarily mapped into **our address space** (kernel-space). 
Trying to **dereference** pathname (**accessing the value** that the pointer pathname is pointing to) 
will result in either a segfault, or garbage data being printed by printk().
Neither of these scenarios are very useful.

To overcome this, the kernel provides us with a bunch of functions like **copy_from_user()**, 
**strncpy_from_user()**, etc, as well as **copy_to_user()** versions for **copying data back into userspace**. 

__Remember: this function is run on kernel-space by a program in user space, that's why
we need to **copy** back and forth from and to user space__

In the snippet above, we are copying a string from pathname, to dir_name, and we will read up to 
NAME_MAX (which is usually 255 - the maximum length of a filename in Linux), or until we hit a 
null-byte (this is the advantage of using **strncpy_from_user()** over the plain old copy_from_user() - 
**it is null-byte aware!**).

Once we’ve got the name of the new folder to-be stored in the dir_name buffer, we can go ahead 
and use printk() with the usual %s format string to print it out to the kernel buffer.

Finally, the most important part is that we actually call orig_mkdir() (the original mkdir) 
with the corresponding arguments.

This ensures that the original functionality of sys_mkdir (i.e. actually creating a new folder) is 
still preserved.

You may be wondering, how is orig_mkdir anything to do with the real sys_mkdir - all we’ve done is define 
it via a function pointer prototype! **Connecting orig_mkdir to the real sys_mkdir is all a part of the 
function hooking process that we’re about to come to**. Notice that, in both cases, **orig_mkdir is defined 
globally**. This allows the hooking/unhooking code in rootkit_init and rootkit_exit to make use of it.

The only thing left is to actually get this function hooked into the kernel in place of the real sys_mkdir!

# Hooking
We’re going to be using **Ftrace** to create a function hook within the kernel,
In practice, we create an **ftrace_hook array**, and then call 
**fh_install_hooks() in rootkit_init() and fh_uninstall_hooks() in rootkit_exit()**. For most practical 
purposes, that’s all you need to know. The real guts of any rootkit is going to be hooks themselves, 
which will be focus of later blog posts. All the functionality we need has been packed into a header 
file called **ftrace_helper.h** by yours truly.

First we need to specify an array that Ftrace will use to handle the hooking for us.
```C
    static struct ftrace_hook hook[] = {
        HOOK("sys_mkdir", hook_mkdir, &orig_mkdir),
    };
```

The `HOOK` **macro** requires the name of the syscall or kernel function that we’re targetting (sys_mkdir),
the hook function we’ve written (hook_mkdir) and the address of where we want the original syscall to be 
**saved** (orig_mkdir).

So, **HOOK** saves the **original** syscall to `orig_mkdir`.

Note that hook[] can contain more than just a single function hook for more complicated rootkits!

Once this array is setup, we use `fh_install_hooks()` to install the function hooks and `fh_remove_hooks()` 
to remove them. All we have to do is put them in the init and exit functions respectively and do a 
little error checking.

It’s time to build! After running make, you should be looking at `irm.ko` sitting in your directory. 
Load it into the kernel with `# insmod irm.ko` and create a new folder with mkdir. 
Then, check `dmesg`.

It’s worth pointing out that we **can only hook functions that are exposed by the kernel**. 
You can see a list of the exposed objects by taking a look at **/proc/kallsyms** (requires root 
otherwise all the memory addresses are 0x0). Clearly, all the syscalls need to be exposed so 
that userspace can get to them, but there are also other functions of interest that aren’t 
syscalls (but still exposed) which we’ll come back to later.

# The Details of ftrace_helper.h
Roughly speaking, one of the features of ftrace is that **it allows us to attach a 
callback to part of the kernel**. 

Specifically, we can **tell ftrace to step in wheneverthe rip register contains a certain 
memory address**. If we set this address to that of sys_mkdir (or any other function) then 
we can cause another function to be executed instead.

To do that, we define **a struct**, named `ftrace_hook`, a **macro** named `HOOK`.

## ftrace_hook
```C
struct ftrace_hook {
    const char *name;
    void *function;
    void *original;

    unsigned long address;
    struct ftrace_ops ops;
};
```


## HOOK:
```C
#define HOOK(_name, _hook, _orig) \
{ \
    .name = SYSCALL_NAME(_name), \
    .function = (_hook), \
    .original = (_orig), \
}
```
For example, if we use it like:
```C
    static struct ftrace_hook hook[] = {
        HOOK("sys_mkdir", hook_mkdir, &orig_mkdir),
    };
```
it simply expands to:
```C
    static struct ftrace_hook hook[] = {
        {
            .name = "sys_mkdir"
            .function = (hook_mkdir),
            .original = (&orig_mkdir),
        },
    };
```
Where the doubled `{}` are used to define an array of structs.
We could have done something like:
```C
    static struct ftrace_hook hook[] = {
        HOOK("sys_mkdir", hook_mkdir, &orig_mkdir),
        HOOK("sys_rmdir", hook_rmdir, &orig_rmdir),
    };
```
and it would have been expanded to:
```C
    static struct ftrace_hook hook[] = {
        {
            .name = "sys_mkdir"
            .function = (hook_mkdir),
            .original = (&orig_mkdir),
        },
        {
            .name = "sys_rmdir"
            .function = (hook_rmdir),
            .original = (&orig_rmdir),
        }
    };
```

`SYSCALL_NAME` is itself a macro that takes care of the fact that, on 64-bit kernels, 
syscalls have `__x64_` prepended to their names.

### about the `.`
The dot `.` in `.name`, `.function`, and `.original` is part of **designated initializers** in C, 
which allows you to explicitly specify the name of the structure fields you're initializing.

## fh_install_hooks()
`fh_install_hooks()` just loops through the `hooks[]` array and calls `fh_install_hook()` on each element.
The first thing that happens is we call `fh_resolve_hook_address()` on the `ftrace_hook` object. 
This function just uses `kallsyms_lookup_name()` (provided by `<linux/kallsyms.h>`) to find the 
address in memory of the real syscall, i.e. `sys_mkdir` in our case. This is important because we 
need to save this both so that we can assign it to `orig_mkdir()` and that we can restore everything 
when the module is unloaded. We save this address into the `.address` field of the `ftrace_hook struct`.

## the wierd preprocessor statement:
```C
#if USE_FENTRY_OFFSET
    *((unsigned long*) hook->original) = hook->address + MCOUNT_INSN_SIZE;
#else
    *((unsigned long*) hook->original) = hook->address;
#endif
```

In kernel development, when hooking a function (intercepting and replacing a system call or other function), 
there's a risk of recursive loops—this happens when the hook calls the original function, 
which in turn triggers the hook again, causing infinite recursion. To avoid this, two methods are often employed:
- Detecting recursion by examining the function return address.
- Skipping the ftrace instrumentation by jumping over the profiling call (which is what MCOUNT_INSN_SIZE refers to).

## FENTRY 1
mcount (or fentry on newer architectures) is a function that is inserted at the start of each function when kernel 
tracing is enabled. It provides a hook for tracing function calls, but this can interfere with function hooking.
