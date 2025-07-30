#include "pins.h"
#include "target.h"
#include "system.h"
#include "motor/motor.h"
#include "hal/gpio.h"
#include "hal/rcc.h"
#include "hal/systick.h"

#include <libopencm3/stm32/exti.h>

int main(void) {

	system_init();

	rcc_clock_setup();

	systick_init();

	gpio_setup_output(LED_GPIO_Port,LED_Pin);

	motor_init(); 
 
	for (;;) {
		gpio_toggle(LED_GPIO_Port,LED_Pin);
		delay_ms(10);
	}
	return 0;
}
