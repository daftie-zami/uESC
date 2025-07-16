CC_PREFIX  ?= arm-none-eabi-
CC      := $(CC_PREFIX)gcc
CXX     := $(CC_PREFIX)g++
LD      := $(CC_PREFIX)gcc
AS      := $(CC_PREFIX)as
OBJCOPY := $(CC_PREFIX)objcopy
OBJDUMP := $(CC_PREFIX)objdump
SIZE    := $(CC_PREFIX)size
GDB     := gdb-multiarch

LDLIBS = -lnosys -L$(MODULE_DIR)/libopencm3/lib $(LIBOPENCM3)

TARGET_NAME := $(PROJECT)_$(TARGET)

ELF := $(BUILD_DIR)/$(TARGET_NAME).elf
BIN := $(BUILD_DIR)/$(TARGET_NAME).bin
HEX := $(BUILD_DIR)/$(TARGET_NAME).hex
MAP := $(BUILD_DIR)/$(TARGET_NAME).map
LSS := $(BUILD_DIR)/$(TARGET_NAME).lss
LIST := $(BUILD_DIR)/$(TARGET_NAME).list

LDFLAGS = $(LDLIBS)\
    -static \
    -nostartfiles \
    -specs=nano.specs \
    -ggdb3 \
    -Wl,--gc-sections \
    -Wl,-Map=$(MAP),--cref \
	-Wl,--print-memory-usage \
    $(ARCH_FLAGS) \
	-L$(MODULE_DIR)/libopencm3/lib -T$(LDSCRIPT)

CFLAGS = $(ARCH_FLAGS)\
    $(addprefix -I,$(INC_DIR)) \
    -D$(PLATFORM) \
    -std=gnu17 \
    -ffreestanding \
    -Wall -Wextra -Werror \
    -fdata-sections \
    -ffunction-sections \
    -ggdb3 -O3 \
    -MMD -MP

OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRC))
$(PROJECT): $(BIN) $(HEX) $(LIST) $(LSS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "$(GREEN)[CC] Compiling\t$(notdir $<)$(NO_COLOR)"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<

$(ELF): $(OBJ)
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)[LD] Linking\t$@$(NO_COLOR)"
	$(Q)$(LD) $(OBJ) $(LDFLAGS) -o $@
	@echo "$(BLUE)[SIZE] Firmware size:$(NO_COLOR)"
	$(Q)$(SIZE) -A -x $@

$(BIN): $(ELF)
	@echo "$(MAGENTA)[OBJCOPY] Creating bin\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJCOPY) -Obinary $< $@

$(HEX): $(ELF)
	@echo "$(MAGENTA)[OBJCOPY] Creating hex\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJCOPY) -Oihex $< $@

$(LIST): $(ELF)
	@echo "$(CYAN)[OBJDUMP] Creating list\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJDUMP) -S $< > $@

$(LSS): $(ELF)
	@echo "$(CYAN)[OBJDUMP] Creating lss\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJDUMP) -h -S $< > $@

-include $(OBJ:.o=.d)