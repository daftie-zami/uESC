# src/bsp/make/STM32G4.mk
ARCH_FLAGS 	:= -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
LIBOPENCM3 	:= -lopencm3_stm32g4
C_DEFINES += USE_CCM_RAM