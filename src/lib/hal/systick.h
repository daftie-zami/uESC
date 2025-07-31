#pragma once

#include "systick/hal_systick.h"

#include <stdint.h>

void systick_init(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);