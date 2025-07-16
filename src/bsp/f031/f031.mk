#FIXME
PLATFORM := STM32F0
LDSCRIPT := $(SRC_DIR)/bsp/f031/linker_f031.ld
LIBOPENCM3 := -lopencm3_stm32f0
ARCH_FLAGS = -mthumb -mcpu=cortex-m3 -msoft-float