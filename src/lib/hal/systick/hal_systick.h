#pragma once

#include <stdint.h>
#include "defs.h"

#define SYSTICK_FREQUENCY ONE_MHZ

void systick_init(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);