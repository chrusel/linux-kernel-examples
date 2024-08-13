| [Home](../README.md) | [Introduction](../section-1/section-1.md) | **File Operations** | [IOCTL](../section-3/section-3.md) | [Access Control and Capabilities](../section-4/section-4.md) | [Misc Driver](../section-5/section-5.md) | [Resources](../section-6/section-6.md)

# Udemy
Chrusel's personal git repository to the character-device-drivers-in-deep udemy course. Linux Kernel 6.5.y. This doc was splitted to one page per section. See [TOC](../README.md).

# 2. File Operations
Header file `linux/cdev.h` defines the cdev struct, whch represents a character device.

        struct cdev {
            struct kobject kobj;
            struct module *owner;
            const struct file_opeations *ops;
            struct list_head list;
            dev_t dev;
            unsigned int count;
        } __randomize_layout;

The cdev funtions are:

        // init a new cdev structure:
        void cdev_init(struct cdev*, const struct file_operations*);

        // allocate and return a cdev structure:
        struct cdev *cdev_alloc(void);

        // add a char-dev to the system:
        int cdev_add(struct cdev*, dev_t, unsigned int minor_count);

        // remove a char-dev from the system
        void cdev_del(struct cdev*);

Difference between `cdev_init(struct cdev*, const struct file_operations*)` and `cdev_alloc(void)`: The cdev_alloc() allocates memory in kernel space, use the cdev_init() when a cdev struct is already allocated.

## 2.1 Register File operations
See example of character device registration in listing [register-fops.c](1-register-fops/register-fops.c).
