# src/bsp/make/STM32F0.mk
ARCH_FLAGS 	:= -mcpu=cortex-m0 -mthumb -msoft-float
LIBOPENCM3 	:= -lopencm3_stm32f0