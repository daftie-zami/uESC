PROJECT = uESC

TARGET ?= 

V ?= 0

PLATFORM ?=

ifneq ($(V), 1)
Q := @
endif

ROOT := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
BUILD_DIR  := $(ROOT)/build
MAKE_DIR   := $(ROOT)/make
MODULE_DIR := $(ROOT)/modules
SRC_DIR    := $(ROOT)/src
TARGET_DIR := $(ROOT)/targets
TOOLS_DIR  := $(ROOT)/tools

# Targets
TARGET_JSONS := $(wildcard $(TARGET_DIR)/*.json)
TARGETS := $(notdir $(basename $(TARGET_JSONS)))

include $(MAKE_DIR)/colors.mk
# include $(SRC_DIR)/bsp/platform/f031/f031.mk

$(TARGETS):
	@echo ">>> Building for target: $@"
	$(MAKE) all TARGET=$@

all:
	@echo ">>> [ALL] Compiling project for $(TARGET)"
	python3 $(TOOLS_DIR)/generate_target_config.py targets/$(TARGET).json $(SRC_DIR)/common/target.h
# 	$(MAKE) $(TARGET)
# 	python3 $(TOOLS_DIR)/gen_linker.py targets/$(TARGET).json src/bsp/linker_template.ld build/$(TARGET)/linker.ld
# 	$(MAKE) -f make/platforms/$(PLATFORM).mk PROJECT=$(PROJECT) TARGET=$(TARGET) LINKER=build/$(TARGET)/linker.ld


clean : 
	@echo "$(RED)[CLEAN] Cleaning up...$(NO_COLOR)"
	@rm -rf $(BUILD_DIR)

lib:
	@echo "$(GREEN)[LIB] Building libraries...$(NO_COLOR)"
	$(Q)$(MAKE) -C $(MODULE_DIR)/libopencm3 || git submodule update --init --recursive

test: 
	@echo "Running tests..."
	$(Q)$(MAKE) -C $(TEST_DIR) test

help:
	@echo "Makefile for project: $(PROJECT)"
	@echo ""
	@echo "Usage:"
	@echo "  make [TARGET=name]           Build given target (e.g., TARGET=bluepill)"
	@echo "  make clean                   Clean all builds"
	@echo ""
	@echo "Available targets (from targets/*.json):"
	@for t in $(TARGETS); do echo "  - $$t"; done


list-targets:
	@echo "Available targets:"
	@$(foreach t,$(TARGETS),echo " - $(t)";)

include $(MAKE_DIR)/Makefile.mk

.DEFAULT_GOAL := help
.PHONY: all clean lib test help list-targets $(TARGETS)