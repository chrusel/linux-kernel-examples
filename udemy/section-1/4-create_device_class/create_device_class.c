#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");

static struct class *class;

static int devfile_init(void)
{
	class = class_create("chruselclass");

    return 0;
}

static void devfile_exit(void)
{
	class_destroy(class);
}

module_init(devfile_init);
module_exit(devfile_exit);
