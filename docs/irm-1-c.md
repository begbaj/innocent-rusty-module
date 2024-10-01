__Tutorial by xcellerator__

# Our First Syscall Hook
With this module, we're going to create a very simple hook for sys_mkdir that prints out the name
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
Note that hook[] can contain more than just a single function hook for more complicated rootkits!
