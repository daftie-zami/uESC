#include "pins.h"
#include "target.h"
#include "system.h"
#include "hal/gpio.h"
#include "hal/rcc.h"
#include "hal/systick.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

__attribute__((unused)) void tim_setup(void)
{
	/* Enable TIM1 clock. */
	rcc_periph_clock_enable(RCC_TIM1);

	/* Enable GPIOA, GPIOB and Alternate Function clocks. */

	// rcc_periph_clock_enable(RCC_AFIO);

	/*
	 * Set TIM1 channel output pins to
	 * 'output alternate function push-pull'.
	 */
	gpio_setup_pin_af(PHASE_U_GPIO_HIGH_GPIO_Port,PHASE_U_GPIO_HIGH_Pin,GPIO_AF6, true);
	gpio_setup_pin_af(PHASE_V_GPIO_HIGH_GPIO_Port,PHASE_V_GPIO_HIGH_Pin,GPIO_AF6, true);
	gpio_setup_pin_af(PHASE_W_GPIO_HIGH_GPIO_Port,PHASE_W_GPIO_HIGH_Pin,GPIO_AF6, true);

	/*
	 * Set TIM1 complementary channel output pins to
	 * 'output alternate function push-pull'.
	 */
	gpio_setup_pin_af(PHASE_U_GPIO_LOW_GPIO_Port,PHASE_U_GPIO_LOW_Pin,GPIO_AF6, true);
	gpio_setup_pin_af(PHASE_V_GPIO_LOW_GPIO_Port,PHASE_V_GPIO_LOW_Pin,GPIO_AF6, true);
	gpio_setup_pin_af(PHASE_W_GPIO_LOW_GPIO_Port,PHASE_W_GPIO_LOW_Pin,GPIO_AF6, true);

	/* Enable TIM1 commutation interrupt. */
	nvic_enable_irq(NVIC_TIM1_TRG_TIM17_IRQ);

	/* Reset TIM1 peripheral. */
	rcc_periph_reset_pulse(RST_TIM1);

	/* Timer global mode:
	 * - No divider
	 * - Alignment edge
	 * - Direction up
	 */
	timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT,
		       TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

	/* Reset prescaler value. */
	timer_set_prescaler(TIM1, 0);

	/* Reset repetition counter value. */
	timer_set_repetition_counter(TIM1, 0);

	/* Enable preload. */
	timer_enable_preload(TIM1);

	/* Continuous mode. */
	timer_continuous_mode(TIM1);

	/* Period (32kHz). */
	timer_set_period(TIM1, get_apb1_freq() / 32000);

	/* Configure break and deadtime. */
	timer_set_deadtime(TIM1, 10);
	timer_set_enabled_off_state_in_idle_mode(TIM1);
	timer_set_enabled_off_state_in_run_mode(TIM1);
	timer_disable_break(TIM1);
	timer_set_break_polarity_high(TIM1);
	timer_disable_break_automatic_output(TIM1);
	timer_set_break_lock(TIM1, TIM_BDTR_LOCK_OFF);

	/* -- OC1 and OC1N configuration -- */

	/* Disable outputs. */
	timer_disable_oc_output(TIM1, TIM_OC1);
	timer_disable_oc_output(TIM1, TIM_OC1N);

	/* Configure global mode of line 1. */
	timer_disable_oc_clear(TIM1, TIM_OC1);
	timer_enable_oc_preload(TIM1, TIM_OC1);
	timer_set_oc_slow_mode(TIM1, TIM_OC1);
	timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);

	/* Configure OC1. */
	timer_set_oc_polarity_high(TIM1, TIM_OC1);
	timer_set_oc_idle_state_set(TIM1, TIM_OC1);

	/* Configure OC1N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC1N);
	timer_set_oc_idle_state_set(TIM1, TIM_OC1N);

	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM1, TIM_OC1, 100);

	/* Reenable outputs. */
	timer_enable_oc_output(TIM1, TIM_OC1);
	timer_enable_oc_output(TIM1, TIM_OC1N);

	/* -- OC2 and OC2N configuration -- */

	/* Disable outputs. */
	timer_disable_oc_output(TIM1, TIM_OC2);
	timer_disable_oc_output(TIM1, TIM_OC2N);

	/* Configure global mode of line 2. */
	timer_disable_oc_clear(TIM1, TIM_OC2);
	timer_enable_oc_preload(TIM1, TIM_OC2);
	timer_set_oc_slow_mode(TIM1, TIM_OC2);
	timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);

	/* Configure OC2. */
	timer_set_oc_polarity_high(TIM1, TIM_OC2);
	timer_set_oc_idle_state_set(TIM1, TIM_OC2);

	/* Configure OC2N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC2N);
	timer_set_oc_idle_state_set(TIM1, TIM_OC2N);

	/* Set the capture compare value for OC1. */
	timer_set_oc_value(TIM1, TIM_OC2, 100);

	/* Reenable outputs. */
	timer_enable_oc_output(TIM1, TIM_OC2);
	timer_enable_oc_output(TIM1, TIM_OC2N);

	/* -- OC3 and OC3N configuration -- */

	/* Disable outputs. */
	timer_disable_oc_output(TIM1, TIM_OC3);
	timer_disable_oc_output(TIM1, TIM_OC3N);

	/* Configure global mode of line 3. */
	timer_disable_oc_clear(TIM1, TIM_OC3);
	timer_enable_oc_preload(TIM1, TIM_OC3);
	timer_set_oc_slow_mode(TIM1, TIM_OC3);
	timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);

	/* Configure OC3. */
	timer_set_oc_polarity_high(TIM1, TIM_OC3);
	timer_set_oc_idle_state_set(TIM1, TIM_OC3);

	/* Configure OC3N. */
	timer_set_oc_polarity_high(TIM1, TIM_OC3N);
	timer_set_oc_idle_state_set(TIM1, TIM_OC3N);

	/* Set the capture compare value for OC3. */
	timer_set_oc_value(TIM1, TIM_OC3, 100);

	/* Reenable outputs. */
	timer_enable_oc_output(TIM1, TIM_OC3);
	timer_enable_oc_output(TIM1, TIM_OC3N);

	/* ---- */

	/* ARR reload enable. */
	timer_enable_preload(TIM1);

	/*
	 * Enable preload of complementary channel configurations and
	 * update on COM event.
	 */
	timer_enable_preload_complementry_enable_bits(TIM1);

	/* Enable outputs in the break subsystem. */
	timer_enable_break_main_output(TIM1);

	/* Counter enable. */
	timer_enable_counter(TIM1);

	/* Enable commutation interrupt. */
	timer_enable_irq(TIM1, TIM_DIER_COMIE);
}

int main(void) {

	system_init();

	rcc_clock_setup();
	systick_init();

	gpio_setup_output(LED_GPIO_Port,LED_Pin);

	// tim_setup(); 
 
	for (;;) {
		gpio_toggle(LED_GPIO_Port,LED_Pin);
		delay_us(20000);
	}
	return 0;
}


void tim1_trg_tim17_isr(void)
{
	static int step = 0;

	/* Clear the COM trigger interrupt flag. */
	timer_clear_flag(TIM1, TIM_SR_COMIF);

	/*
	 * A simplified and inefficient implementation of PWM On
	 * scheme. Look at the implementation in Open-BLDC on
	 * http://open-bldc.org for the proper implementation. This
	 * one only serves as an example.
	 *
	 * Table of the PWM scheme zone configurations when driving:
	 * @verbatim
	 *  | 1| 2| 3| 4| 5| 6|
	 * -+--+--+--+--+--+--+
	 * A|p+|++|  |p-|--|  |
	 * -+--+--+--+--+--+--+
	 * B|  |p-|--|  |p+|++|
	 * -+--+--+--+--+--+--+
	 * C|--|  |p+|++|  |p-|
	 * -+--+--+--+--+--+--+
	 *  |  |  |  |  |  |  '- 360 Deg
	 *  |  |  |  |  |  '---- 300 Deg
	 *  |  |  |  |  '------- 240 Deg
	 *  |  |  |  '---------- 180 Deg
	 *  |  |  '------------- 120 Deg
	 *  |  '----------------  60 Deg
	 *  '-------------------   0 Deg
	 *
	 * Legend:
	 * p+: PWM on the high side
	 * p-: PWM on the low side
	 * --: Low side on
	 * ++: High side on
	 *   : Floating/NC
	 * @endverbatim
	 */
	switch (step) {
	case 0: /* A PWM HIGH, B OFF, C LOW */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FORCE_LOW);

		timer_enable_oc_output(TIM1, TIM_OC1);
		timer_disable_oc_output(TIM1, TIM_OC1N);

		timer_disable_oc_output(TIM1, TIM_OC2);
		timer_disable_oc_output(TIM1, TIM_OC2N);

		timer_enable_oc_output(TIM1, TIM_OC3);
		timer_enable_oc_output(TIM1, TIM_OC3N);

		step++;
		break;
	case 1: /* A HIGH, B PWM LOW, C OFF */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FORCE_HIGH);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FROZEN);

		timer_enable_oc_output(TIM1, TIM_OC1);
		timer_enable_oc_output(TIM1, TIM_OC1N);

		timer_disable_oc_output(TIM1, TIM_OC2);
		timer_enable_oc_output(TIM1, TIM_OC2N);

		timer_disable_oc_output(TIM1, TIM_OC3);
		timer_disable_oc_output(TIM1, TIM_OC3N);

		step++;
		break;
	case 2: /* A OFF, B LOW, C PWM HIGH */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FORCE_LOW);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);

		timer_disable_oc_output(TIM1, TIM_OC1);
		timer_disable_oc_output(TIM1, TIM_OC1N);

		timer_enable_oc_output(TIM1, TIM_OC2);
		timer_enable_oc_output(TIM1, TIM_OC2N);

		timer_enable_oc_output(TIM1, TIM_OC3);
		timer_disable_oc_output(TIM1, TIM_OC3N);

		step++;
		break;
	case 3: /* A PWM LOW, B OFF, C HIGH */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FORCE_HIGH);

		timer_disable_oc_output(TIM1, TIM_OC1);
		timer_enable_oc_output(TIM1, TIM_OC1N);

		timer_disable_oc_output(TIM1, TIM_OC2);
		timer_disable_oc_output(TIM1, TIM_OC2N);

		timer_enable_oc_output(TIM1, TIM_OC3);
		timer_enable_oc_output(TIM1, TIM_OC3N);

		step++;
		break;
	case 4: /* A LOW, B PWM HIGH, C OFF */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FORCE_LOW);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_FROZEN);

		timer_enable_oc_output(TIM1, TIM_OC1);
		timer_enable_oc_output(TIM1, TIM_OC1N);

		timer_enable_oc_output(TIM1, TIM_OC2);
		timer_disable_oc_output(TIM1, TIM_OC2N);

		timer_disable_oc_output(TIM1, TIM_OC3);
		timer_disable_oc_output(TIM1, TIM_OC3N);

		step++;
		break;
	case 5: /* A OFF, B HIGH, C PWM LOW */
		timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_FROZEN);
		timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_FORCE_HIGH);
		timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);

		timer_disable_oc_output(TIM1, TIM_OC1);
		timer_disable_oc_output(TIM1, TIM_OC1N);

		timer_enable_oc_output(TIM1, TIM_OC2);
		timer_enable_oc_output(TIM1, TIM_OC2N);

		timer_disable_oc_output(TIM1, TIM_OC3);
		timer_enable_oc_output(TIM1, TIM_OC3N);

		step = 0;
		break;
	}
	gpio_toggle(LED_GPIO_Port,LED_Pin);
}