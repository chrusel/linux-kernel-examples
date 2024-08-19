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

Steps to create a character driver:

1. Allocate a device number dynamically oder statically (dev-t)
2. Initializing the character device with its file operations (struct cdev, struct file_operations)
3. Registering the character device with the Linux Kernel (cdev_add)

## 2.1 Register File operations
See example of character device registration with `cdev_alloc()` in listing [register-fops.c](1-register-fops-cdev_alloc/register-fops.c).
See the almost same example of character device registration with `cdev_init()` in listing [register-fops.c](2-register-fops-cdev_init/register-fops.c).

    $ sudo insmod register-fops.ko
    $ sudo chmod 666 /dev/chruseldev

Perform a write operation to the character device:

    $ echo "foobar" > /dev/chruseldev
    $ sudo dmesg

Perform a read operation from the character device:

    $ cat /dev/chruseldev
    $ sudo dmesg

    $ sudo rmmod register-fops

## 2.2 Pseudo devices
Devices in Linux that do not correspond to a physical device are so calles Pseudo devices. For example:

`/dev/urandom`: generates a stream of pseudo-random numbers (`$ hexdump /dev/urandom`)

`/dev/null`: accepts and discards any input

`/dev/zero`: generates a stream of \0 characters (`$ dd if=/dev/zero of=1M.txt bs=1M count=1`)

The file drivers/char/mem.c contains the implementation of this devices.

## 2.3 struct file
Is defined in `linux/fs.h` and represents an open file in Kernel space. A file object is created on file open() and will be passed to any function that operates on the file, until the last close. When the counter is zero, the kernel will release the data structure.
A disk file on the other hand is represented by `struct inode`.

        struct file {
            // represents w and/or r
            fmode_t f_mode;

            // the current read or write position
            loff_t f_pos;

            // file flags such as O_RDONLY, O_NONBLOCK, O_SYNC...
            unsigned int	f_flags;

            // the operations associated with the file
            struct file_operations	*f_op;

            // The open system call sets this field to NULL before open is called from the driver.
            // The driver can use this field, but must free memory on release.
            // private_data is a useful resource for preserving state info across system calls
            void *private_data;
        }

See listing [struct_file.c](3-struct-file/struct-file.c).

      # insmod struct-file.ko
      # chmod 666 /dev/chruseldev

      $ ./userapp

## 2.3 struct inode
The sruct inode is used by the kernel internally to represent files. An inode uniquely identifies a file in a file system.

        struct inode {
            // mode
            umode_t i_mode;

            kuid_t i_uid;
            kgid_t i_gid;

            // inode number
            unsigned long i_mode_ino;

            // contains the device_number
            dev_t i_rdev;

            // Kernel representation of char device
            struct cdev *i_mode_cdev;
        }

Two functions can be used to obtain major and minor numbers from an inode:

        iminor(inode)(base) g041840@bw049141-lnx:~/gitrepos/linux-kernel-examples/udemy/section-2$ git commit -s -m "copy_to_user.c: add example kernel driver"

        unsigned int iminor(struct inode *inode);

        imajor(inode)
        unsigned int imajor(struct inode *inode);

See listing [struct_inode.c](4-struct-inode/struct-inode.c). You can read out the inode with the stat tool:

        stat /dev/chruseldev

## 2.4 Accessing Userspace buffer
When you try to access a userspace buffer from kernel space (malfunction [user-buffer.c](5-user-buffer-malfunction/user-buffer.c)) the kernel will most probably seg fault (page domain fault).

Proper access to user-space data is done by calloing the macros / functions below:

        #include <linux/uaccess.h>

        // transfer data of integrated datatypes
        put_user(type val, type *address);
        get_user(type val, type *address);

        // transfer data of structured datatypes
        unsigned long copy_to_user(void __user *to, const void *from, unsigned long n);
        unsigned long copy_from_user(void *to, const void __user *from, unsigned long n);

All macros / functions return 0 in case of success and something else on error.

### 2.4.1 copy_from_user
The `copy_from_user(...)` function copies a block of data from user space into a kernel buffer and desires:

  - to: destination buffer (in kernel space)
  - from: a source buffer (from user space), and
  - n: a length defined in bytes

See listing [copy_from_user.c](6-user-buffer_copy_from_user/copy_from_user.c) for an example.

### 2.4.2 copy_to_user
The `copy_to_user(...)` function copies a block of data from the kernel into user space and desires:

  - to: pointer to a user space buffer
  - from: a pointer to a kernel buffer and
  - n: a length defined in bytes

See listing [copy_to_user.c](7-user-buffer_copy_to_user/copy_to_user.c) for an example.

### 2.4.3 put_user
`jiffies` is a global variable in kernel space which stores the number of clock ticks since boot. It is present in`<linux/jiffies.h>`.
The workstation where the binary was executed increments 1000 jiffies each second. That means, that the timer is configured to generate 1000 interrupts every second.
You can use `put_user()` to copy the value of jiffies to user space. `put_user()` is faster than `copy_to_user()`, and can copy up to 8 bytes of data.
The size that `put_user()` copies depends on the type of the pointer argument and is determined at compile time using `typeof()` and `sizeof()` builtins.

See listing of a character driver which on read returns the value of `jiffies` [put_user.c](8-user-buffer_put_user/put_user.c).

        $ sudo insmod put_user.ko
        $ sudo chmod 666 /dev/myjiffies

### 2.4.4 get_user

