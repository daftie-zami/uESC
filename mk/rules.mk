# ---------------------------------------------------------

ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex
MAP := $(BUILD_DIR)/$(TARGET).map
LSS := $(BUILD_DIR)/$(TARGET).lss
LIST := $(BUILD_DIR)/$(TARGET).list

OBJS = $(C_SRC:%.c=$(BUILD_DIR)/%.o)

_target: $(BIN) $(HEX) $(LIST) $(LSS)

$(BUILD_DIR)/%.o: %.c
	@echo "$(GREEN)[CC] Compiling\t$(notdir $<)$(NO_COLOR)"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<

$(ELF): $(OBJS)
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)[LD] Linking\t$@$(NO_COLOR)"
	$(Q)$(LD) $(OBJS) $(LDFLAGS) -o $@
	@echo "$(WHITE)[SIZE] Firmware size:$(NO_COLOR)"
	$(Q)$(SIZE) -A -x $@

$(BIN): $(ELF)
	@echo "$(BLUE)[OBJCOPY] Creating bin\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJCOPY) -Obinary $< $@

$(HEX): $(ELF)
	@echo "$(BLUE)[OBJCOPY] Creating hex\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJCOPY) -Oihex $< $@

$(LIST): $(ELF)
	@echo "$(MAGENTA)[OBJDUMP] Creating list\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJDUMP) -S $< > $@

$(LSS): $(ELF)
	@echo "$(MAGENTA)[OBJDUMP] Creating lss\t$(notdir $@)$(NO_COLOR)"
	$(Q)$(OBJDUMP) -h -S $< > $@

.PHONY: _target
-include $(OBJS:.o=.d)

# ---------------------------------------------------------