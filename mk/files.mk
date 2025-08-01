# ---------------------------------------------------------

APP_DIR = $(SRC_DIR)/app
COMMON_DIR = $(SRC_DIR)/common
BSP_DIR = $(SRC_DIR)/bsp
LIB_DIR = $(SRC_DIR)/lib

# Source files
C_SRC = \
	$(APP_DIR)/main.c \
	$(BSP_DIR)/system.c \
	$(LIB_DIR)/hal/systick/hal_systick.c \
	$(LIB_DIR)/hal/gpio/hal_gpio.c \
	$(LIB_DIR)/hal/rcc/hal_rcc.c \
	$(LIB_DIR)/pwm_input/pwm_input.c \
	$(LIB_DIR)/dshot/dshot.c \
	$(APP_DIR)/pwm_output.c

# Include directories
INC_DIR = $(SRC_DIR) \
	$(LIBOPENCM3_DIR)/include \
	$(APP_DIR) \
	$(COMMON_DIR) \
	$(BSP_DIR) \
	$(LIB_DIR) \
	$(LIB_DIR)/hal

# ---------------------------------------------------------