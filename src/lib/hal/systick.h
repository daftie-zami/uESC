#pragma once

#include "systick/hal_systick.h"

#include <stdint.h>

extern void systick_init(void);
extern void delay_ms(uint32_t ms);
extern void delay_us(uint32_t us);