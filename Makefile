PROJECT := uESC
TARGET ?=
V ?= 0
DEVICE ?=
# TODO: Make this configurable
OOCD_FILE ?= board/stm32f4discovery.cfg

ROOT 		:= $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
BUILD_DIR 	= $(ROOT)/build
SRC_DIR 	= $(ROOT)/src
OPENCM3_DIR	= $(ROOT)/libopencm3
TARGET_DIR 	= $(SRC_DIR)/target
MAKE_DIR 	= $(ROOT)/mk

CFILES = $(shell find -L $(SRC_DIR) -name '*.c' -exec realpath --relative-to=. {} \;)
AFILES = $(shell find -L $(SRC_DIR) -name '*.S' -exec realpath --relative-to=. {} \;)
INCLUDES += $(patsubst %,-I%, . $(shell find $(SRC_DIR) -type d))

TARGETS := $(notdir $(basename $(wildcard $(TARGET_DIR)/*.json)))

ifdef FAMILY
-include $(MAKE_DIR)/archflags_$(FAMILY).mk
endifd

$(TARGETS):
	@if [ -z "$$NESTED_BUILD" ]; then \
		if [ ! -f "$(TARGET_DIR)/$@.json" ]; then \
			echo "No such target: $@.json"; exit 1; \
		fi; \
		echo "==> Building target: $@"; \
		DEVICE=$$(grep -Po '"device"\s*:\s*"\K[^"]+' $(TARGET_DIR)/$@.json); \
		FAMILY=$$(grep -Po '"family"\s*:\s*"\K[^"]+' $(TARGET_DIR)/$@.json); \
		NESTED_BUILD=1 DEVICE=$$DEVICE FAMILY=$$FAMILY TARGET=$@ $(MAKE) $(MFLAGS) all; \
		exit $$?; \
	fi


all: $(TARGETS)

include $(OPENCM3_DIR)/mk/genlink-config.mk

OPT ?= -Os
CSTD ?= -std=c99


ifeq ($(V),0)
Q	:= @
NULL	:= 2>/dev/null
endif

# Tool paths.
PREFIX	?= arm-none-eabi-
CC	= $(PREFIX)gcc
CXX	= $(PREFIX)g++
LD	= $(PREFIX)gcc
OBJCOPY	= $(PREFIX)objcopy
OBJDUMP	= $(PREFIX)objdump
SIZE	= $(PREFIX)size
OOCD	?= openocd

INCLUDES += $(patsubst %,-I%, . $(OPENCM3_DIR)/include )

OBJS = $(CFILES:%.c=$(BUILD_DIR)/%.o)
OBJS += $(CXXFILES:%.cxx=$(BUILD_DIR)/%.o)
OBJS += $(AFILES:%.S=$(BUILD_DIR)/%.o)

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
LIST := $(BUILD_DIR)/$(TARGET).list
LSS := $(BUILD_DIR)/$(TARGET).lss
MAP := $(BUILD_DIR)/$(TARGET).map
LDSCRIPT := $(BUILD_DIR)/generated.$(DEVICE).ld

GENERATED_BINS = $(ELF) $(BIN) $(MAP) $(LIST) $(LSS)

TGT_CPPFLAGS += -MD
TGT_CPPFLAGS += -Wall -Wundef $(INCLUDES)
TGT_CPPFLAGS += $(INCLUDES) $(OPENCM3_DEFS)


TGT_CFLAGS += $(OPT) $(CSTD) -ggdb3
TGT_CFLAGS += $(ARCH_FLAGS)
TGT_CFLAGS += -fno-common
TGT_CFLAGS += -ffunction-sections -fdata-sections
TGT_CFLAGS += -Wextra -Wshadow -Wno-unused-variable -Wimplicit-function-declaration
TGT_CFLAGS += -Wredundant-decls -Wstrict-prototypes -Wmissing-prototypes

TGT_CXXFLAGS += $(OPT) $(CXXSTD) -ggdb3
TGT_CXXFLAGS += $(ARCH_FLAGS)
TGT_CXXFLAGS += -fno-common
TGT_CXXFLAGS += -ffunction-sections -fdata-sections
TGT_CXXFLAGS += -Wextra -Wshadow -Wredundant-decls  -Weffc++

TGT_ASFLAGS += $(OPT) $(ARCH_FLAGS) -ggdb3

TGT_LDFLAGS += -T$(LDSCRIPT) -L$(OPENCM3_DIR)/lib -nostartfiles
TGT_LDFLAGS += $(ARCH_FLAGS)
TGT_LDFLAGS += -specs=nano.specs
TGT_LDFLAGS += -Wl,--gc-sections
TGT_LDFLAGS += -Wl,-Map=$(MAP)
TGT_LDFLAGS += -Wl,--print-memory-usage
ifeq ($(V),99)
TGT_LDFLAGS += -Wl,--print-gc-sections
endif

# Linker script generator fills this in for us.
ifeq (,$(DEVICE))
LDLIBS += -l$(OPENCM3_LIB)
endif
# nosys is only in newer gcc-arm-embedded...
#LDLIBS += -specs=nosys.specs
LDLIBS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

# Burn in legacy hell fortran modula pascal yacc idontevenwat
.SUFFIXES:
.SUFFIXES: .c .S .h .o .cxx .elf .bin .list .lss

# Bad make, never *ever* try to get a file out of source control by yourself.
%: %,v
%: RCS/%,v
%: RCS/%
%: s.%
%: SCCS/s.%

$(TARGET): $(BIN) $(HEX) $(LIST) $(LSS)

# error if not using linker script generator
ifeq (,$(DEVICE))
$(LDSCRIPT):
ifeq (,$(wildcard $(LDSCRIPT)))
    $(error Unable to find specified linker script: $(LDSCRIPT))
endif
else
# if linker script generator was used, make sure it's cleaned.
GENERATED_BINS += $(LDSCRIPT)
endif

# Compile rules.
$(BUILD_DIR)/%.o: %.c
	@printf "  CC      $(notdir $<)\n"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: %.cxx
	@printf "  CXX     $(notdir $<)\n"
	@mkdir -p $(dir $@)
	$(Q)$(CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.o: %.S
	@printf "  AS      $(notdir $<)\n"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(TGT_ASFLAGS) $(ASFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(ELF): $(OBJS) $(LDSCRIPT) $(LIBDEPS)
	@printf "  LD      $(notdir $@)\n"
	$(Q)$(LD) $(TGT_LDFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	$(Q)$(SIZE) -A -x $@

$(BIN): $(ELF)
	@printf "  BIN     $(notdir $@)\n"
	$(Q)$(OBJCOPY) -O binary  $< $@

$(LSS): $(ELF)
	@printf "  LSS     $(notdir $@)\n"
	$(Q)$(OBJDUMP) -h -S $< > $@

$(LIST): $(ELF)
	@printf "  LIST    $(notdir $@)\n"
	$(Q)$(OBJDUMP) -S $< > $@

clean:
	@printf "  CLEAN   $(BUILD_DIR)\n"
	$(Q)rm -rf $(BUILD_DIR) $(GENERATED_BINS)

.PHONY: clean flash $(TARGETS) all
-include $(OBJS:.o=.d)

include $(OPENCM3_DIR)/mk/genlink-rules.mk
