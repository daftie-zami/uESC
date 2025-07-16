#pragma once

#include <stdint.h>

#define SYSTICK_FREQUENCY 1000000 // 1us

void systick_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);