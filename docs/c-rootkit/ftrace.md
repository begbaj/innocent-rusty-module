# C Ftrace
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

## dynamic tracing
there is the ability to set DYNAMIC_FTRACE, which makes the system run with virtually no overhead.


# resources
- kernel.org 4.16: https://www.kernel.org/doc/html/v4.16/trace/ftrace-uses.html
- kernel.org latest: https://www.kernel.org/doc/html/latest/trace/ftrace.html

