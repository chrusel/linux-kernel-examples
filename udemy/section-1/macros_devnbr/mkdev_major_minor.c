#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>

MODULE_LICENSE("GPL");
static int mkdevmajmin_init(void)
{
    dev_t devicenumber = 10;
    printk("Device Number: %u\n", devicenumber);
    printk("Major Number: %d\n", MAJOR(devicenumber));
    printk("Minor Number: %d\n", MINOR(devicenumber));

    devicenumber = MKDEV(120, 30);
    printk("Device Number: %u\n", devicenumber);
    printk("Major Number : %d\n", MAJOR(devicenumber));
    printk("Minor Number : %d\n", MINOR(devicenumber));

    return 0;
}

static void mkdevmajmin_exit(void)
{
}

module_init(mkdevmajmin_init);
module_exit(mkdevmajmin_exit);
