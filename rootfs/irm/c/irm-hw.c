#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL"); // necessario per abilitare l'importazione di altri
                       // moduli e funzioni sotto questa licenza
MODULE_AUTHOR("Began Bajrami");
MODULE_DESCRIPTION("IRM-C-HW");
MODULE_VERSION("1.0");

static int __init rootkit_init(void) {
  printk(KERN_INFO "Hello, World!\n");
  return 0;
}

static void __exit rootkit_exit(void) {
  printk(KERN_INFO "I'm just a chill module\n");
}

module_init(rootkit_init); // funzione che viene eseguita all'inserimento del
                           // modulo (con insmod)
module_exit(rootkit_exit); // funzione che viene eseguita alla rimozione del
                           // modulo (con rmmod)
