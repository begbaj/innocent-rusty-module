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

## __user identifier
This keyword is used in the Linux kernel to indicate that this pointer refers to memory in **user space** 
(where regular programs run). 
The kernel has to be careful when accessing user-space memory, as it operates in a different space than 
user programs.

