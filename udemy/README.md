# Udemy
Chrusel's personal git repository to the character-device-drivers-in-deep udemy course.

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
Example kernel module: [mkdev_major_minor.c](section-1/1-macros_devnbr/mkdev_major_minor.c)
## 1.2 Static device number allocation
Static device number allocation: [static_allocation_devnbr.c](section-1/2-static_allocation/static_allocation_devnbr.c)

        # rmmod static_allocation_devnbr.ko
        # insmod static_allocation_devnbr.ko major_number=123 device_name=chrusel_foobar

Multiple drivers developers may choose the same major/minor number, also statically assignment of device numbers is probably not the best! The better solution would be making the assignement of the device number dynamically. Anyway, it is okay for the linux kernel to have multiple chardevs with identical device name, The number-pair maj/min must be uniq. Maximum Major number is 511 (artificial limit set in linux/fs.h) and maximum Minor number is 2^20 (1048576), device-number is a 32-bit value.
## 1.3 Dynamic device number allocation
Dynamic device number allocation: [dynamic_allocation_devnbr.c](section-1/3-dynamic_allocation/dynamic_allocation_devnbr.c)
## 1.4 Create Device File
### 1.4.1 Manual
        # mknod -m <permissions> <name> <device-type> <major> <minor>
        $ sudo mknod -m 644 /dev/chruseldev c 244 10
        $ ls -l /dev/chruseldev
### 1.4.2 Automatic
class_create: is defined in file `linux/device.h`. Udev will take notice at creating or destroying such a class. [create_device_class.c](section-1/4-create_device_class/create_device_class.c)


        struct class *class_create(const char* name); // API changed since udemy course recording

        name: pointer to a string for the name of this class

        --> will create the directory /sys/class/<name>/

        void class_destroy(struct class *cls);

device_create: is defined in file `linux/device.h`: [create_device_file.c](section-1/5-create_device_file/create_device_file.c)

        struct device *device_create(struct class *class,
                                     struct device *parent,
                                     dev_t devt,
                                     void *drv_data,
                                     const char *fmt,
                                     ...);

        void device_destroy(struct class *class, dev_t devt);                         

# 2. File Operations
# 3. IOCTL
