# ---------------------------------------------------------

define generate_upin_header
	$(Q)python3 $(TOOLS_DIR)/upin_to_header.py \
		$(1) \
		$(2)
endef

define generate_utarget_header
	$(Q)python3 $(TOOLS_DIR)/utarget_to_header.py \
		$(1)
endef

define generate_openocd_cfg
	$(Q)python3 $(TOOLS_DIR)/openocd_cfg_gen.py \
		--interface $(1) \
		--target $(2) \
		--build-dir $(3)
endef

# ---------------------------------------------------------