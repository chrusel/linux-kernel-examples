# Udemy
Chrusel's personal git repository to the character-device-drivers-in-deep udemy course.

# 1. Introduction
## 1.1 Use of macros MKDEV, MAJOR, MINOR
  - Example kernel module: [mkdev_major_minor.c](section-1/1-macros_devnbr/mkdev_major_minor.c)
  - Static device number allocation: [static_allocation_devnbr.c](section-1/2-static_allocation/static_allocation_devnbr.c)



        $ rmmod static_allocation_devnbr.ko
        $ insmod static_allocation_devnbr.ko major_number=123 device_name=chrusel_foobar

# 2. File Operations
# 3. IOCTL
