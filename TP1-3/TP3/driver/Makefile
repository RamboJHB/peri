CARD_NUMB       = 22
ROUTER          = 132.227.102.36
LOGIN           = Jiang_Lee
LAB             = lab3

MODULE          = lcdJL

CROSSDIR        = /users/enseig/franck/peri
KERNELDIR       = /dsk/l1/misc/linux-rpi-3.18.y
CROSS_COMPILE   = $(CROSSDIR)/arm-bcm2708hardfp-linux-gnueabi/bin/bcm2708hardfp-
        
obj-m           += $(MODULE).o

default:;       
	make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
exe : test.x
	scp -P50$(CARD_NUMB) test.x pi@$(ROUTER):$(LOGIN)/$(LAB)
test.x: test.c
	bcm2708hardfp-gcc -o $@ $< -O2 -static
clean:;         
	make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) clean
upload:;        
	scp -P50$(CARD_NUMB) $(MODULE).ko pi@$(ROUTER):$(LOGIN)/$(LAB)
