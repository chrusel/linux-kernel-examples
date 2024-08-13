| [Home](../README.md) | **Introduction** | [File Operations](../section-2/section-2.md) | [IOCTL](../section-3/section-3.md) | [Access Control and Capabilities](../section-4/section-4.md) | [Misc Driver](../section-5/section-5.md) | [Resources](../section-6/section-6.md)

# Udemy
Chrusel's personal git repository to the character-device-drivers-in-deep udemy course. Linux Kernel 6.5.y. This doc was splitted to one page per section. See [TOC](../README.md).

# 1. Introduction
Information about all the devic-files and its numbers are listed in `/proc/devices`

        $ cat /proc/devices

ls -l also gives the maj and the minor number back of a device file.

        $ ls -l /dev/

list all character devices:

        $ ls -l /dev/ | grep ^c

list all block devices:

        $ ls -l /dev/ | grep ^b

## 1.1 Use of macros MKDEV, MAJOR, MINOR
Example kernel module: [mkdev_major_minor.c](1-macros_devnbr/mkdev_major_minor.c)
## 1.2 Static device number allocation
Static device number allocation: [static_allocation_devnbr.c](2-static_allocation/static_allocation_devnbr.c)

        # rmmod static_allocation_devnbr.ko
        # insmod static_allocation_devnbr.ko major_number=123 device_name=chrusel_foobar

Multiple drivers developers may choose the same major/minor number, also statically assignment of device numbers is probably not the best! The better solution would be making the assignement of the device number dynamically. Anyway, it is okay for the linux kernel to have multiple chardevs with identical device name, The number-pair maj/min must be uniq. Maximum Major number is 511 (artificial limit set in linux/fs.h) and maximum Minor number is 2^20 (1048576), device-number is a 32-bit value.
## 1.3 Dynamic device number allocation
Dynamic device number allocation: [dynamic_allocation_devnbr.c](3-dynamic_allocation/dynamic_allocation_devnbr.c)
## 1.4 Create Device File
### 1.4.1 Manual
        # mknod -m <permissions> <name> <device-type> <major> <minor>
        $ sudo mknod -m 644 /dev/chruseldev c 244 10
        $ ls -l /dev/chruseldev
### 1.4.2 Automatic
The function `class_create()`/`class_destroy()` are defined in file `linux/device.h`. Udev will take notice at creating or destroying such a class. See listiong [create_device_class.c](4-create_device_class/create_device_class.c) for example.


        struct class *class_create(const char* name); // API changed since udemy course recording

        name: pointer to a string for the name of this class

        --> will create the directory /sys/class/<name>/

        void class_destroy(struct class *cls);

The function `device_create()`/`device_destroy()` are defined in file `linux/device.h`. See listing [create_device_file.c](5-create_device_file/create_device_file.c) for example.

        struct device *device_create(struct class *class,
                                     struct device *parent,
                                     dev_t devt,
                                     void *drv_data,
                                     const char *fmt,
                                     ...);

        void device_destroy(struct class *class, dev_t devt);

