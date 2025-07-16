#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencmsis/core_cm3.h>
#include "system.h"

static void system_clock_setup(void);
static void systick_init(void);

extern uint32_t rcc_ahb_frequency;
volatile uint32_t systick_delay = 0;

void system_init(void) {
    system_clock_setup();
    systick_init();
}

static void system_clock_setup(void){
	#if defined(STM32F0)
	rcc_clock_setup_in_hsi_out_48mhz(); /* Set up the system clock */
	#else
	#error "Unsupported STM32 family"
	#endif
}

static void systick_init(void) {
	// systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
    // systick_set_reload(rcc_ahb_frequency / 8 / SYSTICK_FREQUENCY - 1);
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(rcc_ahb_frequency / SYSTICK_FREQUENCY - 1);
    systick_counter_enable();
}

uint32_t systick_get_counter(void) { return systick_delay; }

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
	

void sys_tick_handler(void) { systick_delay++; }