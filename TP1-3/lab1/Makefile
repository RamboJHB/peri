NAME = Jiang_Lee
RPI = 22 

CROSS_COMPILE ?= bcm2708hardfp-

CFLAGS=-Wall -Wfatal-errors -O2
LDFLAGS=-static -L. -lgpio
LDFLAGS=-static  

help:
	@echo ""
	@echo "make [RPI=num] target"
	@echo ""
	@echo "      target :	help	this message"
	@echo " 		all	clean all and recompile"
	@echo " 		dep	compile if required"
	@echo " 		clean	clean all except sources"
	@echo " 		copy	compile then copy to card $(RPI) in directory $(NAME)"
	@echo ""

all: clean dep

dep : lab1_ex1.x

one : one_blink.x

two : two_blink.x

but : button.x

copy: *.x
	scp -P 50$(RPI) $^ pi@132.227.102.36:$(NAME)

%.x: %.c
	$(CROSS_COMPILE)gcc -o $@ $^ $(LDFLAGS)

libgpio.a: gpio_read.o gpio_config.o gpio_setup.o gpio_write.o
	$(CROSS_COMPILE)ar -rcs $@ $^

%.o: %.c
	$(CROSS_COMPILE)gcc -o $@ -c $(CFLAGS) $<

clean:
	rm -f *.o *.x *~ *.a
