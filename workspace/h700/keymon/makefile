ifeq (,$(CROSS_COMPILE))
$(error missing CROSS_COMPILE for this toolchain)
endif

ifeq (,$(PLATFORM))
PLATFORM=$(UNION_PLATFORM)
endif

TARGET = keymon
PRODUCT = $(TARGET).elf

CC = $(CROSS_COMPILE)gcc
CFLAGS	= -Os -lmsettings -lpthread -lrt -ldl -Wl,--gc-sections -s
CFLAGS  += -I. -I../../all/common -I../platform/ -I$(PREFIX_LOCAL)/include -L$(PREFIX_LOCAL)/lib -DPLATFORM=\"$(UNION_PLATFORM)\"

all: $(PREFIX_LOCAL)/include/msettings.h
	$(CC) $(TARGET).c -o $(PRODUCT) $(CFLAGS)
clean:
	rm -rf $(PRODUCT)

# Use the checkout-relative library target, not the container's mount path.
$(PREFIX_LOCAL)/include/msettings.h:
	$(MAKE) -C ../libmsettings build
