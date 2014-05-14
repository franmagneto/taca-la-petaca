#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Francisco Neto");

#define VIDEO_LENGTH 5986482

char video_data[] = {
#include "video.h"
};

static int __init taca_init(void) {
	printk(KERN_ALERT "Hello, world\n");
	return 0;
}

static void __exit taca_exit(void) {
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(taca_init);
module_exit(taca_exit);
