# cc1: warning: switch '-mcpu=cortex-a8' conflicts with switch '-march=armv7-a'
# https://stackoverflow.com/a/53143269
ARCH = armv7-a
#MCPU = cortex-a8
TARGET = rvpb

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy

LINKER_SCRIPT = ./navilos.ld
MAP_FILE = build/navilos.map

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.os, $(ASM_SRCS))

# SECTION_START ============================================
# Usually we should modify(or add) this section when we add new source(or header)
VPATH = boot			\
		hal/$(TARGET)	\
		lib

INC_DIRS =	-I includes			\
			-I hal				\
			-I hal/$(TARGET)	\
			-I lib

C_SRCS = $(notdir $(wildcard boot/*.c))
C_SRCS+= $(notdir $(wildcard hal/$(TARGET)/*.c))
C_SRCS+= $(notdir $(wildcard lib/*.c))
C_OBJS = $(patsubst %.c, build/%.o, $(C_SRCS))
# SECTION_END ============================================

C_FLAGS = -c -g -std=c11

navilos = build/navilos.axf
navilos_bin = build/navilos.bin

.PHONY: all clean run debug gdb

all: $(navilos)

clean:
	@rm -rf build

run: $(navilos)
	qemu-system-arm -M realview-pb-a8 -display none -kernel $(navilos)\
		-nographic

debug: $(navilos)
	qemu-system-arm -M realview-pb-a8 -display none -kernel $(navilos)\
		-nographic -S -gdb tcp::1234,ipv4

gdb:
	arm-none-eabi-gdb

$(navilos): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(navilos) $(ASM_OBJS)\
		$(C_OBJS) -Map=$(MAP_FILE)
	$(OC) -O binary $(navilos) $(navilos_bin)

build/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) $(INC_DIRS) $(C_FLAGS) -o $@ $<

build/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) $(INC_DIRS) $(C_FLAGS) -o $@ $<
 