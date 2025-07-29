# ---------------------------------------------------------

ARCH_FLAGS  ?= -mcpu=cortex-m3 -mthumb -msoft-float
LIBOPENCM3  ?= -lopencm3_stm32f1
PLATFORM    ?= STM32F1

LDLIBS := -lc -lgcc -lnosys -lm

C_DEFINES +=

CFLAGS = $(ARCH_FLAGS) \
    $(addprefix -I,$(INC_DIR)) \
    $(addprefix -D,$(C_DEFINES)) \
    -D$(PLATFORM) \
    -std=gnu17 \
    -ggdb3 -Oz \
    -ffreestanding \
    -Wall -Wextra -Werror \
    -fdata-sections \
    -ffunction-sections \
    -MMD -MP

LDFLAGS = $(LDLIBS) \
    -L$(LIBOPENCM3_DIR)/lib $(LIBOPENCM3) \
    -ggdb3 \
    -static \
    -nostartfiles \
    -specs=nano.specs \
    -Wl,--gc-sections \
    -Wl,--print-memory-usage \
    $(ARCH_FLAGS) \
    -Wl,-Map=$(MAP),--cref \
    -L$(LIBOPENCM3_DIR)/lib -T$(LDSCRIPT)

# ---------------------------------------------------------