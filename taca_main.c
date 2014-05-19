/*
 * Taca la Petaca - Módulo de kernel
 * =================================
 *
 * Autor: Francisco Machado Magalhães Neto
 *
 * Maio de 2014
 *
 * Módulo para o kernel do Linux que cria o dispositivo `/dev/taca`, que
 * contém o vídeo *Chapolin - Taca La Petaca*, e a entrada `/proc/taca`,
 * com a letra da música.
 *
 * O código foi baseado no álbum *Cycles Per Instruction*, da banda
 * [netcat](http://www.netcat.co/), lançado como módulo de kernel.
 *
 * Também foi usado como referência o livro *Linux Device Drivers*.
 */

#define pr_fmt(msg) "[" KBUILD_MODNAME "]: " msg

#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>

#include "taca.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Francisco Machado Magalhães Neto");
MODULE_DESCRIPTION("Este módulo cria o dispositivo /dev/taca, \
que contém o vídeo \"Chapolin - Taca La Petaca\" \
(http://www.youtube.com/watch?v=Nq-6IaeDP48), e a entrada /proc/taca, \
que contém a letra da música (http://letras.mus.br/chaves/1095878/)");

/**
 * Estrutura comum para conter os dados a serem lidos
 * (Será inicializada com os dados do vídeo para o dispositivo,
 * e com a letra para a entrada /proc/taca
 **/
struct taca {
	char *data;
	ssize_t len;
};

/**
 * Estrutura que contém os dados do vídeo e a letra
 **/
extern struct taca_video video;

/**
 * Abre o dispositivo /dev/taca, inicializando um "struc taca"
 * com os dados do vídeo e armazenando em file->private_data
 */
static int taca_open_device(struct inode *inode, struct file *file) {

	struct taca *taca;

	taca = kzalloc(sizeof(*taca), GFP_KERNEL);
	if (!taca)
		return -ENOMEM;

	taca->data = video.data;
	taca->len = video.data_len;
	file->private_data = taca;
	return 0;
}

/**
 * Abre a entrada /proc/taca, inicializando um "struc taca"
 * com os dados da letra e armazenando em file->private_data
 */
static int taca_open_proc(struct inode *inode, struct file *file) {

	struct taca *taca;

	taca = kzalloc(sizeof(*taca), GFP_KERNEL);
	if (!taca)
		return -ENOMEM;

	taca->data = video.lyrics;
	taca->len = video.lyrics_len;
	file->private_data = taca;
	return 0;
}

/**
 * Função de fechamento de arquivo comum a /dev/taca e /proc/taca,
 * que desaloca a estrutura em file->private_data
 */
static int taca_release(struct inode *inode, struct file *file) {

	struct taca *taca = file->private_data;

	kfree(taca);
	return 0;
}

/**
 * Função de leitura comum a /dev/taca e /proc/taca.
 * O conteúdo a ser lido depende do valor de file->private_data
 * atribuído na função de abertura
 */
static ssize_t taca_read(struct file *file, char *buffer, size_t length, loff_t *offset) {

	struct taca *taca = file->private_data;

	if (*offset + length > taca->len)
		length -= *offset + length - taca->len;

	if (copy_to_user(buffer, taca->data + *offset, length))
		return -EFAULT;

	*offset += length;
	return length;
}

/**
 * Ambos /dev/taca e /proc/taca não suportam escrita
 */
static ssize_t taca_write(struct file *file, const char *buffer, size_t length, loff_t *offset) {

	pr_err("Escrita não suportada.\n");
	return -EINVAL;
}

/**
 * Operações de arquivo para /dev/taca
 */
static const struct file_operations device_fops = {
	.owner = THIS_MODULE,
	.read = taca_read,
	.write = taca_write,
	.open = taca_open_device,
	.release = taca_release
};

/**
 * Estrutura miscdevice de /dev/taca, com as operações de arquivo
 * definidas em device_ops
 */
static struct miscdevice taca_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = KBUILD_MODNAME,
	.fops = &device_fops,
	.mode = S_IRUGO
};

/**
 * Operações de arquivo para /proc/taca
 */
static const struct file_operations proc_fops = {
	.owner = THIS_MODULE,
	.read = taca_read,
	.write = taca_write,
	.open = taca_open_proc,
	.release = taca_release
};

struct proc_dir_entry *proc_entry;
int err;

/**
 * Função de inicialização do módulo, onde os arquivos especiais
 * /dev/taca e /proc/taca são criados
 */
static int __init taca_init(void) {

	err = misc_register(&taca_dev);
	if (err) {
		pr_err("Dispositivo /dev/" KBUILD_MODNAME " não criado\n");
		return err;
	}

	proc_entry = proc_create(KBUILD_MODNAME, S_IRUGO, NULL, &proc_fops);
	if (proc_entry == NULL) {
		pr_err("Entrada /proc/" KBUILD_MODNAME " não criada\n");
		misc_deregister(&taca_dev);
		return -ENOMEM;
	}

	pr_info("Taca la Petaca - Módulo de kernel");
	pr_info("Autor: Francisco Machado Magalhães Neto");
	pr_info("Para reproduzir, utilize o comando 'ffplay /dev/taca'");
	pr_info("Letra da música disponível no arquivo '/proc/taca'");

	return 0;
}

/**
 * Função executada quando o módulo é descarregado.
 * Remove os arquivos especiais /dev/taca e /proc/taca
 */
static void __exit taca_exit(void) {

	if (!err) {
		misc_deregister(&taca_dev);
	}

	if (proc_entry) {
		proc_remove(proc_entry);
	}

	pr_info("Módulo removido");
}

module_init(taca_init);
module_exit(taca_exit);
