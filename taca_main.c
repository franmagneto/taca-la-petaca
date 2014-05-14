#define pr_fmt(msg) "[" KBUILD_MODNAME "]: " msg

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "taca.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Francisco Machado Magalhães Neto");
MODULE_DESCRIPTION("Módulo que toca o vídeo \"Chapolin - Taca La Petaca\"");

struct taca {
	char *position;
};

extern struct taca_video video;

static int device_open(struct inode *inode, struct file *file) {

	struct taca *taca;

	taca = kzalloc(sizeof(*taca), GFP_KERNEL);
	if (!taca) {
		return -ENOMEM;
	}
	taca->position = video.data;
	file->private_data = taca;
	return 0;
}

static int device_release(struct inode *inode, struct file *file) {

	struct taca *taca = file->private_data;

	kfree(taca);
	return 0;
}

static ssize_t device_read(struct file *file, char *buffer, size_t length, loff_t *offset) {

	struct taca *taca = file->private_data;
	int bytes = 0;

	while (length && (taca->position - video.data) < video.len) {

		put_user(*(taca->position++), buffer++);

		length--;
		bytes++;
	}

	return bytes;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {

	pr_err("Escrita não suportada.\n");
	return -EINVAL;
}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_release,
	.read = device_read,
	.write = device_write
};

static struct miscdevice taca_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = KBUILD_MODNAME,
	.fops = &fops,
	.mode = S_IRUGO
};

static int __init taca_init(void) {

	int err;

	err = misc_register(&taca_dev);
	if (err) {
		pr_err("Falha ao registrar dispositivo\n");
		return err;
	}

	pr_info("Dispositivo /dev/" KBUILD_MODNAME " criado\n");

	return 0;
}

static void __exit taca_exit(void) {

	misc_deregister(&taca_dev);
	pr_info("Dispositivo /dev/" KBUILD_MODNAME " removido\n");
}

module_init(taca_init);
module_exit(taca_exit);
