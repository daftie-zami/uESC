#FIXME
FLASH_SIZE := 128K # 128K, 256K, 512K
RAM_SIZE := 128K # fixed size for G474
PLATFORM := STM32F0
LDSCRIPT := $(SRC_DIR)/bsp/platform/f031/linker_f031.ld
LIBOPENCM3 := -lopencm3_stm32f0
ARCH_FLAGS = -mthumb -mcpu=cortex-m3 -msoft-float