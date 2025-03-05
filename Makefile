# cc1: warning: switch '-mcpu=cortex-a8' conflicts with switch '-march=armv7-a'
# https://stackoverflow.com/a/53143269
ARCH = armv7-a
#MCPU = cortex-a8

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy

LINKER_SCRIPT = ./navilos.ld

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.o, $(ASM_SRCS))

INC_DIRS = includes

navilos = build/navilos.axf
navilos_bin = build/navilos.bin

.PHONY: all clean run debug gdb

all: $(navilos)

clean:
	@rm -rf build

run: $(navilos)
	qemu-system-arm -M realview-pb-a8 -display none -kernel $(navilos)

debug: $(navilos)
	qemu-system-arm -M realview-pb-a8 -display none -kernel $(navilos)\
		-S -gdb tcp::1234,ipv4

gdb:
	arm-none-eabi-gdb

$(navilos): $(ASM_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(navilos) $(ASM_OBJS)
	$(OC) -O binary $(navilos) $(navilos_bin)

build/%.o: boot/%.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -I$(INC_DIRS) -c -g -o $@ $<

 