#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DATA_LENGTH 5986482

static int __init taca_init(void) {
	printk(KERN_ALERT "Hello, world\n");
	return 0;
}

static void __exit taca_exit(void) {
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(taca_init);
module_exit(taca_exit);
