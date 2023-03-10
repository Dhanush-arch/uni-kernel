# Default values for build system.
export V         ?=
export ARCH      ?= x64
export BUILD_DIR ?= build
export RELEASE   ?=
ifeq ($(shell uname), Darwin)
LLVM_PREFIX ?= /usr/local/opt/llvm/bin/
GRUB_PREFIX ?= /usr/local/opt/i386-elf-grub/bin/i386-elf-
endif

# The default build target.
.PHONY: default
default: build

# Disable builtin implicit rules and variables.
MAKEFLAGS += --no-builtin-rules --no-builtin-variables
.SUFFIXES:

# Enable verbose output if $(V) is set.
ifeq ($(V),)
.SILENT:
endif

include src/arch/$(ARCH)/build.mk
include test/$(ARCH)/build.mk

sources += $(wildcard src/*.c)
sources += $(wildcard src/fs/*.c)
sources += $(wildcard src/drivers/*.c)
sources += $(wildcard src/arch/$(ARCH)/*.c)
sources += $(wildcard src/arch/$(ARCH)/*.S)
objs := $(patsubst %.S,$(BUILD_DIR)/%.o, $(patsubst %.c,$(BUILD_DIR)/%.o, $(sources)))

test_sources += $(wildcard test/$(ARCH)/*.S)
test_sources += $(wildcard test/$(ARCH)/*.c)
test_objs := $(patsubst %.S,$(BUILD_DIR)/%.o, $(patsubst %.c,$(BUILD_DIR)/%.o, $(test_sources)))

CC       := $(LLVM_PREFIX)clang$(LLVM_SUFFIX)
LD       := $(LLVM_PREFIX)ld.lld$(LLVM_SUFFIX)
OBJCOPY  := $(LLVM_PREFIX)llvm-objcopy$(LLVM_SUFFIX)
PYTHON3  := python3
CARGO    := cargo
PROGRESS := printf "  \\033[1;96m%8s\\033[0m  \\033[1;m%s\\033[0m\\n"

QEMUFLAGS += -nographic -serial mon:stdio
QEMUFLAGS += -no-reboot -d cpu_reset,unimp,guest_errors
QEMUFLAGS += -drive id=blk1,if=none,file=$(BUILD_DIR)/disk.qcow2,format=qcow2
QEMUFLAGS += -device virtio-blk-pci,drive=blk1,disable-legacy=off,disable-modern=on
QEMUFLAGS += $(if $(GDB),-S -s,)

CFLAGS += -g3 -std=c11 -ffreestanding -fno-builtin -nostdlib -nostdinc
CFLAGS += -fPIE -fstack-size-section
CFLAGS += -Isrc -Isrc/arch/$(ARCH)
CFLAGS += -Wall -Wextra
CFLAGS += -Werror=implicit-function-declaration
CFLAGS += -Werror=int-conversion
CFLAGS += -Werror=incompatible-pointer-types
CFLAGS += -Werror=shift-count-overflow
CFLAGS += -Werror=switch
CFLAGS += -Werror=return-type
CFLAGS += -Werror=pointer-integer-compare
CFLAGS += -Werror=tautological-constant-out-of-range-compare
CFLAGS += -Werror=visibility
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-gnu-variable-sized-type-not-at-end
CFLAGS += -DGIT_COMMIT='"$(shell git rev-parse --short HEAD)"'

LDFLAGS += -pie --no-dynamic-linker -static -Map build/boot2dump.map

ifeq ($(RELEASE),)
CFLAGS += -O0 -fsanitize=undefined
else
CFLAGS += -Oz -DRELEASE
endif

#
#  Build Commands
#
.PHONY: build
build: $(BUILD_DIR)/boot2dump.bin $(BUILD_DIR)/compile_commands.json

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: run
run: $(BUILD_DIR)/test_os.elf $(BUILD_DIR)/disk.qcow2
	cp $(BUILD_DIR)/test_os.elf $(BUILD_DIR)/test_os.qemu.elf
	./tools/make-bootable-on-qemu.py $(BUILD_DIR)/test_os.qemu.elf
	$(PROGRESS) "RUN" $<
	$(QEMU) $(QEMUFLAGS) -kernel $(BUILD_DIR)/test_os.qemu.elf

.PHONY: rust-build
rust-build: $(BUILD_DIR)/boot2dump.bin
	cp $(BUILD_DIR)/boot2dump.bin rust
	$(PROGRESS) "CARGO" rust
	cd rust && $(CARGO) build --release

.PHONY: rust-publish
rust-publish: $(BUILD_DIR)/boot2dump.bin
	cp $(BUILD_DIR)/boot2dump.bin rust
	$(PROGRESS) "CARGO" rust
	cd rust && $(CARGO) publish --allow-dirty

#
#  Build Rules
#
$(BUILD_DIR)/boot2dump.bin: $(BUILD_DIR)/boot2dump.elf
	$(PROGRESS) "OBJCOPY" $@
	$(OBJCOPY) -Obinary $< $@

$(BUILD_DIR)/boot2dump.elf: $(objs)
	$(PROGRESS) "LD" $@
	$(LD) $(LDFLAGS) -o $@ $(objs)

$(BUILD_DIR)/%.o: %.c Makefile
	$(PROGRESS) "CC" $<
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $< -MD -MF $(@:.o=.deps) -MJ $(@:.o=.json)

$(BUILD_DIR)/%.o: %.S Makefile $(boot_elf)
	$(PROGRESS) "CC" $<
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $< -MD -MF $(@:.o=.deps) -MJ $(@:.o=.json)

$(BUILD_DIR)/test/x64/test_boot.o: $(BUILD_DIR)/boot2dump.bin
$(BUILD_DIR)/test_os.elf: $(test_objs)
	$(PROGRESS) "LD" $@
	$(LD) $(TEST_LDFLAGS) -o $@ $(test_objs)

$(BUILD_DIR)/disk.qcow2: ~/.cache/boot2dump/focal-server-cloudimg-amd64.qcow2
	$(PROGRESS) "CP" $@
	cp $< $@
	echo "HELLO FROM BOOT.DUMP" > build/boot.dump
	sudo virt-copy-in -a $@ build/boot.dump /

~/.cache/boot2dump/focal-server-cloudimg-amd64.qcow2:
	$(PROGRESS) "WGET" focal-server-cloudimg-amd64.qcow2
	mkdir -p ~/.cache/boot2dump
	wget -O $@ https://cloud-images.ubuntu.com/focal/20211202/focal-server-cloudimg-amd64.img

# JSON compilation database.
# https://clang.llvm.org/docs/JSONCompilationDatabase.html
$(BUILD_DIR)/compile_commands.json: $(objs)
	$(PROGRESS) "GEN" $(BUILD_DIR)/compile_commands.json
	$(PYTHON3) tools/merge-compile-commands.py \
		-o $(BUILD_DIR)/compile_commands.json \
		$(shell find $(BUILD_DIR) -type f -name "*.json")

# Build dependencies generated by clang and Cargo.
-include $(shell find $(BUILD_DIR) -name "*.deps" 2>/dev/null)
