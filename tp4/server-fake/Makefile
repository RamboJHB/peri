CARD_NUMB       = 26
ROUTER          = 132.227.102.36
LOGIN           = huang-jiang
LAB             = tp4

MODULE          = ledbp_JH

CROSSDIR        = /users/enseig/franck/peri
KERNELDIR       = /dsk/l1/misc/linux-rpi-3.18.y
CROSS_COMPILE   = $(CROSSDIR)/arm-bcm2708hardfp-linux-gnueabi/bin/bcm2708hardfp-
        
obj-m           += $(MODULE).o
default:;       make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
clean:;         make -C $(KERNELDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) clean
upload:;        scp -P50$(CARD_NUMB) $(MODULE).ko test2.c pi@$(ROUTER):$(LOGIN)/$(LAB)

copy: rmdev insdev
	scp -P 5026 $^ pi@132.227.102.36:huang-jiang/tp4
