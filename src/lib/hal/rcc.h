#pragma once

#include <stdint.h>

#include "rcc/hal_rcc.h"

uint32_t get_ahb_freq(void);
uint32_t get_apb1_freq(void);
uint32_t get_apb2_freq(void);
void rcc_clock_setup(void);