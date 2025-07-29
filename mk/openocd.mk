OPENOCD_CFG := $(BUILD_DIR)/openocd.cfg
DEBUG_ELF := $(BUILD_DIR)/debug.elf

debug: flash
	@echo "$(YELLOW)[DEBUG] Starting OpenOCD...$(NO_COLOR)"
	$(Q)openocd -f $(OPENOCD_CFG)

flash:
	@echo "$(YELLOW)[OPENOCD] Flashing $(DEBUG_ELF)$(NO_COLOR)"
	$(Q)openocd -f $(OPENOCD_CFG) -c "program $(DEBUG_ELF) verify reset exit"

debug-gdb:
	$(Q) $(GDB) \
	-ex "target extended-remote :3333" \
	-ex "monitor reset halt" \
	-ex "break main" \
	-ex "continue" \
	-ex "layout split" \
	$(DEBUG_ELF)

.PHONY: debug flash debug-gdb