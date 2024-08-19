obj-m += user-buffer.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

userapp:
	gcc -o userapp userapp.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f userapp
