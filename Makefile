# ---------------------------------------------------------

PROJECT := uESC

TARGET ?=

DEVICE ?=

PLATFORM ?=

TOOLCHAIN ?= gcc  # or 'llvm'

DEBUG_PROBE ?= STLINK

V ?= 1

# ---------------------------------------------------------

ifneq ($(V),1)
	Q := @
endif

# ---------------------------------------------------------

BUILD_DIR      := build
MK_DIR    	   := mk
MODULE_DIR 	   := modules
SRC_DIR    	   := src
TEST_DIR   	   := test
TOOLS_DIR  	   := tools
TARGET_DIR     := $(SRC_DIR)/targets
LIBOPENCM3_DIR := libopencm3

# ---------------------------------------------------------

include $(MK_DIR)/files.mk
include $(MK_DIR)/colors.mk
include $(MK_DIR)/flags.mk
include $(MK_DIR)/rules.mk
include $(MK_DIR)/sysdiag.mk
include $(MK_DIR)/toolchain.mk
include $(MK_DIR)/openocd.mk
include $(MK_DIR)/tools.mk

# ---------------------------------------------------------

TARGETS := $(notdir $(wildcard $(TARGET_DIR)/*))

ifneq ($(TARGET),)
PLATFORM := $(shell awk -F= '/^PLATFORM/ {gsub(/ /, "", $$2); print $$2}' $(TARGET_DIR)/$(TARGET)/$(TARGET).utarget)
DEVICE := $(shell awk -F= '/^DEVICE/ {gsub(/ /, "", $$2); print $$2}' $(TARGET_DIR)/$(TARGET)/$(TARGET).utarget)
LDSHORT := $(shell echo $(DEVICE) | cut -c1-9)
DEVICE_SUFFIX := $(shell echo $(DEVICE) | sed 's/^.*\(.\{3\}\)$$/\1/')
FLASH_SIZE_LETTER := $(shell echo $(DEVICE_SUFFIX) | cut -c1)
LDSUFFIX := x$(FLASH_SIZE_LETTER)
include $(SRC_DIR)/bsp/make/$(PLATFORM).mk
endif

# TODO
all: $(TARGETS)

_target_prepare:
	$(info [INFO] Makefile configures $(PLATFORM) platform settings for $(TARGET))
	$(Q)$(call generate_openocd_cfg,$(DEBUG_PROBE),$(DEVICE),$(BUILD_DIR))
	$(Q)$(call generate_upin_header, $(TARGET_DIR)/$(TARGET)/$(TARGET).upin, $(SRC_DIR)/common/pins.h)
	$(Q)$(call generate_utarget_header, $(TARGET_DIR)/$(TARGET)/$(TARGET).utarget)
	$(Q)$(MAKE) --no-print-directory _target
	$(Q)cp $(ELF) $(BUILD_DIR)/debug.elf

$(TARGETS): %:
	$(Q)$(MAKE) --no-print-directory TARGET=$@ _target_prepare

# ---------------------------------------------------------

clean:
	@echo "$(RED)[CLEAN] Cleaning build dir$(NO_COLOR)"
	$(Q)rm -rf $(BUILD_DIR)

test: 
	@echo "Running tests..."
	$(Q)$(MAKE) -C $(TEST_DIR) TARGET=$@ _target --no-print-directory

help:
	@echo "Makefile for $(PROJECT)"

.DEFAULT_GOAL := help
.PHONY: all _target_prepare $(TARGETS) clean test help

# ---------------------------------------------------------