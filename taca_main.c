#define pr_fmt(msg) "[" KBUILD_MODNAME "]: " msg

#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>

#include "taca.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Francisco Machado Magalhães Neto");
MODULE_DESCRIPTION("Módulo que toca o vídeo \"Chapolin - Taca La Petaca\"");

struct taca {
	char *data;
	char *position;
	ssize_t length;
};

extern struct taca_video video;

static int taca_open_device(struct inode *inode, struct file *file) {

	struct taca *taca;

	taca = kzalloc(sizeof(*taca), GFP_KERNEL);
	if (!taca)
		return -ENOMEM;

	taca->data = video.data;
	taca->position = video.data;
	taca->length = video.length;
	file->private_data = taca;
	return 0;
}

static int taca_open_proc(struct inode *inode, struct file *file) {

	struct taca *taca;

	taca = kzalloc(sizeof(*taca), GFP_KERNEL);
	if (!taca)
		return -ENOMEM;

	taca->data = video.lyrics;
	taca->position = video.lyrics;
	taca->length = strlen(video.lyrics);
	file->private_data = taca;
	return 0;
}

static int taca_release_generic(struct inode *inode, struct file *file) {

	struct taca *taca = file->private_data;

	kfree(taca);
	return 0;
}

static ssize_t taca_read_generic(struct file *file, char *buffer, size_t length, loff_t *offset) {

	struct taca *taca = file->private_data;
	int bytes = 0;

	while (length && (taca->position - taca->data) < taca->length) {

		put_user(*(taca->position++), buffer++);

		length--;
		bytes++;
	}

	return bytes;
}

static ssize_t taca_write_generic(struct file *file, const char *buffer, size_t length, loff_t *offset) {

	pr_err("Escrita não suportada.\n");
	return -EINVAL;
}

static const struct file_operations device_fops = {
	.owner = THIS_MODULE,
	.read = taca_read_generic,
	.write = taca_write_generic,
	.open = taca_open_device,
	.release = taca_release_generic
};

static struct miscdevice taca_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = KBUILD_MODNAME,
	.fops = &device_fops,
	.mode = S_IRUGO
};

static const struct file_operations proc_fops = {
	.owner = THIS_MODULE,
	.read = taca_read_generic,
	.write = taca_write_generic,
	.open = taca_open_proc,
	.release = taca_release_generic
};

static int __init taca_init(void) {

	int err;

	err = misc_register(&taca_dev);
	if (err) {
		pr_err("Dispositivo /dev/" KBUILD_MODNAME " não criado\n");
		return err;
	}
	pr_info("Dispositivo /dev/" KBUILD_MODNAME " criado\n");

	proc_create(KBUILD_MODNAME, S_IRUGO, NULL, &proc_fops);
	pr_info("Entrada /proc/" KBUILD_MODNAME " criada\n");

	return 0;
}

static void __exit taca_exit(void) {

	remove_proc_entry(KBUILD_MODNAME, NULL);
	pr_info("Entrada /proc/" KBUILD_MODNAME " removida\n");
	misc_deregister(&taca_dev);
	pr_info("Dispositivo /dev/" KBUILD_MODNAME " removido\n");
}

module_init(taca_init);
module_exit(taca_exit);
