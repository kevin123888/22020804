CROSS_COMPILE := arm-linux-gnueabihf-
#要调用开发版源码编译
ifeq ($(arch),arm)

KERNELDIR := /home/ubuntu/linux-5.4.31
CC := $(CROSS_COMPILE)gcc
else 
#ubuntu 下的调试
KERNELDIR := /lib/modules/$(shell uname -r)/build
CC:=gcc
endif

PWD := $(shell pwd)
modname ?=

all:
	make -C $(KERNELDIR) M=$(PWD) modules   
	$(CC) test.c -pthread -o test
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm test
install:
	sudo cp *.ko ~/nfs/rootfs/home/drivers
	sudo cp test ~/nfs/rootfs/home/app
obj-m := $(modname).o
$(modname)-objs := mycdev.o led.o si7006.o mp74hc595.o fan.o



