#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

int base_minor = 0;
char * device_name = "chruseldev";
int count = 1;
dev_t devicenumber;

static struct class *class = NULL;
static struct device *device = NULL;
static struct cdev mycdev;

MODULE_LICENSE("GPL");

static int device_open(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);

    // print f_flags	
	if ((file->f_flags & O_ACCMODE) == O_RDONLY) {
		pr_info("  Opened File in Read only mode\n");
	} else if ((file->f_flags & O_ACCMODE) == O_WRONLY) {
		pr_info("  Opened File in Write only mode\n");
	} else if ((file->f_flags & O_ACCMODE) == O_RDWR) {
		pr_info("  Opened File in Read/Write mode\n");
	}

	if (file->f_flags & O_CREAT) {
		pr_info("  Create if it does't exist\n");
	}

	if (file->f_flags & O_EXCL) {
		pr_info("  Provide exclusive access\n");
	}

	if (file->f_flags & O_TRUNC) {
		pr_info("  Truncate the file to zero size first\n");
	}

	if (file->f_flags & O_APPEND) {
		pr_info("  Append to the file (don't overwrite)\n");
	}

	if (file->f_flags & O_NONBLOCK) {
		pr_info("  Access methods are non-blocking\n");
	}

	if (file->f_flags & O_SYNC) {
		pr_info("  O_SYNC\n");
	}

	pr_info("  File Offset: %llu\n", file->f_pos);

	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	pr_info("%s\n", __func__);
	return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
	return 0;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *offset)
{
	pr_info("%s\n", __func__);
	return count;
}

struct file_operations device_fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
};


static int hello_init(void)
{
	class = class_create("chruselclass");
	
	if (!alloc_chrdev_region(&devicenumber, base_minor, count, device_name)) {
		printk("Device number registered\n");
		printk("Major number received: %d\n", MAJOR(devicenumber));
		
		device = device_create(class, NULL, devicenumber, NULL, device_name);
		
		// initialize chardev
		cdev_init(&mycdev, &device_fops);
		mycdev.owner = THIS_MODULE;
		cdev_add(&mycdev, devicenumber, count);

	} else
		printk("Device number registration failed!");

    return 0;
}

static void hello_exit(void)
{
	device_destroy(class, devicenumber);
	class_destroy(class);
	cdev_del(&mycdev);
	unregister_chrdev_region(devicenumber, count);
	return;
}

module_init(hello_init);
module_exit(hello_exit);

