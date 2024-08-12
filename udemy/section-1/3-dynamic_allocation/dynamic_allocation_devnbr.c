#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

int base_minor = 0;
char *device_name = "chrusel_chardev";
int count = 1;
dev_t devicenumber;

module_param(base_minor, int, 0);
module_param(count, int, 0);
module_param(device_name, charp, 0);

MODULE_LICENSE("GPL");
static int dynalloc_init(void)
{
    devicenumber = MKDEV(120, 30);
    printk("Device Number: %u\n", devicenumber);
    printk("Major Number : %d\n", MAJOR(devicenumber));
    printk("Minor Number : %d\n", MINOR(devicenumber));
    printk("Count        : %d\n", count);
    printk("Device Name  : %s\n", device_name);
    
    if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name)) {
    	printk("Device number registered\n");
    	printk("Major number received: %d\n", MAJOR(devicenumber));
    } else
	    printk("Device number registration failed\n");

    return 0;
}

static void dynalloc_exit(void)
{
	unregister_chrdev_region(devicenumber, count);
}

module_init(dynalloc_init);
module_exit(dynalloc_exit);
