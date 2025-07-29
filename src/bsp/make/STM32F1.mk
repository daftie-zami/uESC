# src/bsp/make/STM32F1.mk
ARCH_FLAGS 	:= -mcpu=cortex-m3 -mthumb -msoft-float
LIBOPENCM3 	:= -lopencm3_stm32f1