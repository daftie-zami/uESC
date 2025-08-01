# src/bsp/make/STM32L0.mk
ARCH_FLAGS := -mcpu=cortex-m0plus -mthumb -msoft-float
LIBOPENCM3 := -lopencm3_stm32l0
C_DEFINES += USE_EEPROM