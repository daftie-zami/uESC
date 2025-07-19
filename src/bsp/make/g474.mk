FLASH_SIZE := 128K # 128K, 256K, 512K
RAM_SIZE := 128K # fixed size for G474
PLATFORM := STM32G4
LDSCRIPT := $(SRC_DIR)/platform/g474/linker_g474.ld
LIBOPENCM3 := -lopencm3_stm32g4
ARCH_FLAGS = -mthumb -mcpu=cortex-m4 -msoft-float