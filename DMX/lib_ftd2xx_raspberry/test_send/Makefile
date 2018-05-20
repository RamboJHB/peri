DEPENDENCIES := -lftd2xx -lpthread

UNAME := $(shell uname)
# Assume target is Mac OS if build host is Mac OS; any other host targets Linux
ifeq ($(UNAME), Darwin)
	DEPENDENCIES += -lobjc -framework IOKit -framework CoreFoundation
else
	DEPENDENCIES += -lrt
endif

# Embed in the executable a run-time path to libftd2xx
LINKER_OPTIONS := -Wl,-rpath /usr/local/lib

CFLAGS = -Wall -Wextra $(DEPENDENCIES) $(LINKER_OPTIONS) -L/usr/local/lib

APP = write

all: $(APP)

$(APP): main.c	
	$(CC) main.c -o $(APP) $(CFLAGS)

test: bitbang.c	
	$(CC) bitbang.c -o bitbang $(CFLAGS)	
	
clean:
	-rm -f *.o ; rm $(APP)
rm:
	sudo rmmod ftdi_sio
	sudo rmmod usbserial
