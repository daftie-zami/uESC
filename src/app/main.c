#include <libopencm3/stm32/gpio.h>
#include "system.h"

int main(void)
{
	system_init();

	for (;;) {
		delay_ms(1000); // Delay for 1 second
		gpio_toggle(GPIOA, GPIO5); // Toggle GPIO pin PA5 (assuming it's configured as output)
		// Add any other application logic here
	}

	return 0;
}