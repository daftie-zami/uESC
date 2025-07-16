#include "systick.h"
#include <libopencm3/cm3/systick.h>
#include <libopencmsis/core_cm3.h>

extern uint32_t rcc_ahb_frequency;

void systick_init(void) {
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(rcc_ahb_frequency / SYSTICK_FREQUENCY - 1);
    systick_counter_enable();
}

void delay_us(uint32_t us) {
	uint32_t start = SysTick->VAL;
    uint32_t ticks = us;
    uint32_t reload = SysTick->LOAD + 1;

    uint32_t elapsed = 0;
    while (elapsed < ticks) {
        uint32_t now = SysTick->VAL;

        if (now <= start)
            elapsed += (start - now);
        else
            elapsed += (start + (reload - now));

        start = now;
    }
}

void delay_ms(uint32_t ms) {
	while(ms--){
		delay_us(1000); // Call delay_us to achieve 1 ms delay
	}
}