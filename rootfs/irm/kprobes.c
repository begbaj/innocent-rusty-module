#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>

//===============================================================================

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Filip Pynckels");
MODULE_DESCRIPTION("Test module for: Unexported kallsyms_lookup_name from Linux kernel 5.7.7 on");
MODULE_VERSION("0.01");

//===============================================================================



static unsigned long lookup_name(const char *name){
	struct kprobe kp;
	unsigned long retval;

	kp.symbol_name = name;
	if (register_kprobe(&kp) < 0) return 0;
	retval = (unsigned long)kp.addr;
	unregister_kprobe(&kp);
	return retval;
}

static int __init kallsyms_init(void)
{
	int (*fnct1)(unsigned long param);
	int (*fnct2)(unsigned long param);

	printk(KERN_INFO "kallsyms: initializing...\n");
	printk(KERN_INFO "kallsyms: initialized\n");

	fnct1 = (void *)lookup_name("page_is_ram");
	fnct2 = (void *)lookup_name("phys_mem_access_prot");

	printk(KERN_INFO "kallsyms: option(3) page_is_ram(%p) phys_mem_access_prot(%p)\n", fnct1, fnct2);

	return 0;
}


static void __exit kallsyms_exit(void)
{
	printk(KERN_INFO "kallsyms: exiting...\n");
	printk(KERN_INFO "kallsyms: exited\n");
}

//===============================================================================

module_init(kallsyms_init);
module_exit(kallsyms_exit);
