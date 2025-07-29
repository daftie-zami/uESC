#pragma once

#include <stdint.h>

#define SYSTICK_FREQUENCY 1000000 //us delay 1000 * 1000

void systick_init(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);