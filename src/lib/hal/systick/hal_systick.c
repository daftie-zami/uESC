#include "systick.h"
#include "hal/rcc.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

void systick_init(void) {
    /*
     * SysTick clock = AHB_clock
     * Reload value = AHB_clock / (SYSTICK_FREQUENCY) - 1
     */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(get_ahb_freq() / SYSTICK_FREQUENCY - 1);
    systick_clear();
    systick_counter_enable();
}

void delay_us(uint32_t us) {
    while (us--) {
        uint32_t start = systick_get_value();
        uint32_t ticks = systick_get_reload(); // 1 tick = 1us, so wait 1 tick
        while ((start - systick_get_value()) < ticks);
    }
}

void delay_ms(uint32_t ms) {
    while (ms--) {
        delay_us(1000);
    }
}

