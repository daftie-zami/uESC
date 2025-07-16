PROJECT = uESC

TARGET ?= default_target

TARGETS := 

# Q := @

ROOT := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
BUILD_DIR  := $(ROOT)/build
MAKE_DIR   := $(ROOT)/make
MODULE_DIR := $(ROOT)/modules
SRC_DIR    := $(ROOT)/src

include $(MAKE_DIR)/colors.mk
include $(SRC_DIR)/bsp/f031/f031.mk

# Source files
C_SRC = $(shell find -L $(SRC_DIR) -name '*.c')

# Include directories
INC_DIR = $(shell find $(SRC_DIR) -type d) \
	$(MODULE_DIR)/libopencm3/include

all: $(PROJECT)

clean : 
	@echo "$(RED)[CLEAN] Cleaning up...$(NO_COLOR)"
	@rm -rf $(BUILD_DIR)

lib:
	@echo "$(GREEN)[LIB] Building libraries...$(NO_COLOR)"
	$(Q)$(MAKE) -C $(MODULE_DIR)/libopencm3

test: 
	@echo "Running tests..."
	$(Q)$(MAKE) -C $(TEST_DIR) test

help:
	@echo "Makefile for $(PROJECT)"
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "$(PROJECT) --- Build the project"

include $(MAKE_DIR)/Makefile.mk

.DEFAULT_GOAL := help
.PHONY: all clean lib test help