PROJECT := uESC
TARGET ?=
V ?= 1
DEVICE ?= dummy_device
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
$(info Available targets: $(TARGETS))
# ifdef FAMILY
# -include $(MAKE_DIR)/archflags_$(FAMILY).mk
# endif

$(TARGETS):
	@if [ -z "$$NESTED_BUILD" ]; then \
		if [ ! -f "$(TARGET_DIR)/$@.json" ]; then \
			echo "No such target: $@.json"; exit 1; \
		fi; \
		DEVICE=$$(grep -Po '"device"\s*:\s*"\K[^"]+' $(TARGET_DIR)/$@.json); \
		FAMILY=$$(grep -Po '"family"\s*:\s*"\K[^"]+' $(TARGET_DIR)/$@.json); \
		echo "DEVICE is: $$DEVICE"; \
		echo "FAMILY is: $$FAMILY"; \
		NESTED_BUILD=1 DEVICE=$$DEVICE FAMILY=$$FAMILY TARGET=$@ $(MAKE) $(MFLAGS) _make_target; \
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

include $(OPENCM3_DIR)/mk/gcc-config.mk
OOCD	:= openocd

INCLUDES += $(patsubst %,-I%, . $(OPENCM3_DIR)/include)

OBJS = $(CFILES:%.c=$(BUILD_DIR)/%.o)
OBJS += $(CXXFILES:%.cxx=$(BUILD_DIR)/%.o)
OBJS += $(AFILES:%.S=$(BUILD_DIR)/%.o)

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
LIST := $(BUILD_DIR)/$(TARGET).list
LSS := $(BUILD_DIR)/$(TARGET).lss
MAP := $(BUILD_DIR)/$(TARGET).map
LDSCRIPT := $(BUILD_DIR)/generated.$(DEVICE).ld

GENERATED_BINS = $(ELF) $(BIN) $(MAP) $(LIST) $(LSS) $(LDSCRIPT)

TGT_CFLAGS += $(OPT) $(CSTD) -ggdb3
TGT_CFLAGS += $(ARCH_FLAGS)
TGT_CFLAGS += -fno-common
TGT_CFLAGS += -ffunction-sections -fdata-sections
TGT_CFLAGS += -Wextra -Wshadow -Wno-unused-variable -Wimplicit-function-declaration
TGT_CFLAGS += -Wredundant-decls -Wstrict-prototypes -Wmissing-prototypes

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

_make_target: $(BIN) $(HEX) $(LIST) $(LSS)

# Compile rules.
$(BUILD_DIR)/%.o: %.c
	@printf "  CC      $(notdir $<)\n"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

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

help:
	@printf "  HELP    Makefile targets:\n"
	@printf "  all     Build all targets\n"
	@printf "  clean   Remove build directory and generated files\n"
	@printf "  flash   Flash the target to the device using OpenOCD\n"
	@printf "  $(TARGETS) Build the specified target\n"
	@printf "  help    Show this help message\n"
 
 .DEFAULT_GOAL := help
.PHONY: clean flash $(TARGETS) all _make_target help
-include $(OBJS:.o=.d)

include $(OPENCM3_DIR)/mk/genlink-rules.mk
