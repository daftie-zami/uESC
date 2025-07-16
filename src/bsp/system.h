#pragma once

#define SYSTICK_FREQUENCY 1000000 // 1us

void system_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
uint32_t systick_get_counter(void);