override MAKEFLAGS += -rR

# Default target config
TARGET_ARCH := x86_64
PUBLIC := public
TARGET_BOOTLOADER := limine

# ARCH Check
SUPPORTED_ARCHS := x86_64

ifeq ($(filter $(TARGET_ARCH),$(SUPPORTED_ARCHS)),)
    $(error Error: Leaf currently doesn't support $(TARGET_ARCH))
else
    include arch/$(TARGET_ARCH)/config.mk
endif

# Target defs
TARGET_FULL := $(TARGET_ARCH)-$(TARGET_FORMAT)
TARGET_PATH := arch/$(TARGET_ARCH)
TARGET_ROOT := $(TARGET_PATH)/$(PUBLIC)

KERNEL := Leaf
KERNEL_DIR := kernel

# Set correct output values for flags coresponding to arch
ifeq ($(TARGET_ARCH),x86_64)
    CC_OUT := x86-64
	CC_VER := 64
	LD_OUT := elf_x86_64
    NASM_OUT := elf64
else
    $(error Error: Leaf currently doesn't support $(TARGET_ARCH))
endif

# Tools
CC := $(TARGET_FULL)-gcc
AS := $(TARGET_FULL)-as
NASM := nasm
LD := $(TARGET_FULL)-ld
LD_CONF := $(TARGET_PATH)/linker.ld

# Tool Flags
CC_FLAGS := \
	-g \
	-O2 \
	-pipe \
	-O0 \
	-I$(TARGET_PATH) \
	-I$(KERNEL_DIR) \
	-Wall \
	-Wextra \
	-Werror \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIE \
	-fno-PIC \
	-m$(CC_VER) \
	-march=$(CC_OUT) \
	-mabi=sysv \
	-mcmodel=kernel \
	-mno-80387 \
	-mno-red-zone \
    -msse \
    -msse2 \
    -Wimplicit-function-declaration \
    -Wdiv-by-zero \
    -Wunused-variable 

# Leaf C Definitions
CC_FLAGS += -DLEAF_ARCH=\"$(TARGET_ARCH)\"
CC_FLAGS += -DLEAF_BOOTLOADER=\"$(TARGET_BOOTLOADER)\"

LD_FLAGS := \
	-nostdlib \
	-static  \
	-m $(LD_OUT) \
	-z max-page-size=0x1000 \
	-T $(LD_CONF)

NASM_FLAGS := \
	-Wall \
	-f $(NASM_OUT)
	

# Files
CFILES := $(shell cd $(KERNEL_DIR) && find -L * -type f -name '*.c')
ASFILES := $(shell cd $(KERNEL_DIR) && find -L * -type f -name '*.S')
NASMFILES := $(shell cd $(KERNEL_DIR) && find -L * -type f -name '*.asm')
OBJ := $(addprefix build/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
HEADER_DEPS := $(addprefix build/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

# Targets
.PHONY: all
all: hdd

bin/$(KERNEL).bin: Makefile $(LD_CONF) $(OBJ)
	@printf "  LD\t$@\n"
	@mkdir -p "$$(dirname $@)"
	@$(LD) $(OBJ) $(LD_FLAGS) -o $@

-include $(HEADER_DEPS)

build/%.c.o: $(KERNEL_DIR)/%.c Makefile
	@printf "  CC\t$<\n"
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CC_FLAGS) -c $< -o $@

build/%.S.o: $(KERNEL_DIR)/%.S Makefile
	@printf "  AS\t$<\n"
	@mkdir -p "$$(dirname $@)"
	@$(CC) $(CC_FLAGS) -c $< -o $@

build/%.asm.o: $(KERNEL_DIR)/%.asm Makefile
	@printf "  AS\t$<\n"
	@mkdir -p "$$(dirname $@)"
	@nasm $(NASM_FLAGS) $< -o $@

# ISO Build target
ISO_OUT_DIR := release
ISO_OUT := $(ISO_OUT_DIR)/Leaf-$(TARGET_ARCH)-$(shell date +%B-%Y).iso
ISO_DIR := public/
SYSROOT := $(TARGET_ROOT)

.PHONY: hdd
hdd: $(ISO_OUT)

$(ISO_OUT): bin/$(KERNEL).bin
	@if [ "$(TARGET_ARCH)" != "x86_64" ]; then \
		echo "Error: ISO generation is only supported for x86_64 target" > /dev/stderr; \
		exit 1; \
	fi
	@mkdir -p $(ISO_OUT_DIR) > /dev/null 2>&1
	@mkdir -p $(ISO_DIR) > /dev/null 2>&1
	@mkdir -p $(ISO_DIR)/boot > /dev/null 2>&1
	@cp -v $(SYSROOT)/* $(ISO_DIR) > /dev/null 2>&1
	@cp -v bin/$(KERNEL).bin $(ISO_DIR)/boot/leaf > /dev/null 2>&1
	@make -C $(TARGET_PATH)/limine > /dev/null 2>&1
	@cp -v $(TARGET_PATH)/limine/limine-bios.sys $(TARGET_PATH)/limine/limine-bios-cd.bin $(TARGET_PATH)/limine/limine-uefi-cd.bin $(ISO_DIR) > /dev/null 2>&1
	@mkdir -p $(ISO_DIR)/EFI/BOOT > /dev/null 2>&1
	@cp -v $(TARGET_PATH)/limine/BOOTX64.EFI $(ISO_DIR)/EFI/BOOT/ > /dev/null 2>&1
	@cp -v $(TARGET_PATH)/limine/BOOTIA32.EFI $(ISO_DIR)/EFI/BOOT/ > /dev/null 2>&1
	@xorriso -as mkisofs -b limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label $(ISO_DIR) -o $@ > /dev/null 2>&1
	@$(TARGET_PATH)/limine/limine bios-install $@ > /dev/null 2>&1
	@rm -rf $(ISO_DIR) > /dev/null 2>&1

# Clean target
.PHONY: clean
clean:
	@rm -rf bin build $(ISO_OUT_DIR)