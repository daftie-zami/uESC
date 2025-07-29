# src/bsp/make/STM32F3.mk
ARCH_FLAGS  := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
LIBOPENCM3 := -lopencm3_stm32f3
