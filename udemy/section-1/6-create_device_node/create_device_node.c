#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>

int base_minor = 0;
char * device_name = "chruseldev";
int count = 1;
dev_t devicenumber;

static struct class *class;
static struct device *device;

MODULE_LICENSE("GPL");

static int node_init(void)
{
	class = class_create("chruselclass");
	
	if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name)) {
		printk("Device number registered\n");
		printk("Major number received: %d\n", MAJOR(devicenumber));
		device = device_create(class, NULL, devicenumber, NULL, device_name);
	} else
		printk("Device number registration failed!");

    return 0;
}

static void node_exit(void)
{
	unregister_chrdev_region(devicenumber, count);
	device_destroy(class, devicenumber);
	class_destroy(class);
	
	return;
}

module_init(node_init);
module_exit(node_exit);
