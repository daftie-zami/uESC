#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include "systick.h"
#include "utils.h"
#include "target.h"

static void system_clock_setup(void){
#if defined(STM32F0) // STM32F0xx family
#if defined(USE_HSE8)
	rcc_clock_setup_in_hse_8mhz_out_48mhz(); // Set up the system clock using HSE at 8 MHz to output 48 MHz
#else
	rcc_clock_setup_in_hsi_out_48mhz(); // Set up the system clock using HSI at 48 MHz
#endif
#else
	#error "Unsupported STM32 family"
#endif
}

static void system_init(void) {
	system_clock_setup();
	systick_init();
}

int main(void)
{
	system_init();

	for (;;) {
		delay_us(1000); //
	}

	return 0;
}