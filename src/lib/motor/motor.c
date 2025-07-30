#include "motor.h"
#include "pins.h"
#include "target.h"
#include "hal/gpio.h"
#include "hal/rcc.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

void motor_init(void){

	rcc_periph_clock_enable(MOTOR_TIM_RCC);

	gpio_setup_pin_af(PHASE_U_GPIO_HIGH_GPIO_Port,PHASE_U_GPIO_HIGH_Pin,GPIO_AF6, true);
	gpio_setup_pin_af(PHASE_V_GPIO_HIGH_GPIO_Port,PHASE_V_GPIO_HIGH_Pin,GPIO_AF6, true);
	gpio_setup_pin_af(PHASE_W_GPIO_HIGH_GPIO_Port,PHASE_W_GPIO_HIGH_Pin,GPIO_AF6, true);

	gpio_setup_pin_af(PHASE_U_GPIO_LOW_GPIO_Port,PHASE_U_GPIO_LOW_Pin,GPIO_AF6, true);
	gpio_setup_pin_af(PHASE_V_GPIO_LOW_GPIO_Port,PHASE_V_GPIO_LOW_Pin,GPIO_AF6, true);
	gpio_setup_pin_af(PHASE_W_GPIO_LOW_GPIO_Port,PHASE_W_GPIO_LOW_Pin,GPIO_AF6, true);

	nvic_enable_irq(NVIC_TIM1_TRG_TIM17_IRQ);

	rcc_periph_reset_pulse(MOTOR_TIM_RST);

	timer_set_mode(MOTOR_TIM, TIM_CR1_CKD_CK_INT,
		       TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

	timer_set_prescaler(MOTOR_TIM, 0);

	timer_set_repetition_counter(MOTOR_TIM, 0);

	timer_enable_preload(MOTOR_TIM);

	timer_continuous_mode(MOTOR_TIM);

	timer_set_period(MOTOR_TIM, get_apb1_freq() / 32000);

	timer_set_deadtime(MOTOR_TIM, 10);
	timer_set_enabled_off_state_in_idle_mode(MOTOR_TIM);
	timer_set_enabled_off_state_in_run_mode(MOTOR_TIM);
	timer_disable_break(MOTOR_TIM);
	timer_set_break_polarity_high(MOTOR_TIM);
	timer_disable_break_automatic_output(MOTOR_TIM);
	timer_set_break_lock(MOTOR_TIM, TIM_BDTR_LOCK_OFF);

	timer_disable_oc_output(MOTOR_TIM, TIM_OC1);
	timer_disable_oc_output(MOTOR_TIM, TIM_OC1N);

	timer_disable_oc_clear(MOTOR_TIM, TIM_OC1);
	timer_enable_oc_preload(MOTOR_TIM, TIM_OC1);
	timer_set_oc_slow_mode(MOTOR_TIM, TIM_OC1);
	timer_set_oc_mode(MOTOR_TIM, TIM_OC1, TIM_OCM_PWM1);

	timer_set_oc_polarity_high(MOTOR_TIM, TIM_OC1);
	timer_set_oc_idle_state_set(MOTOR_TIM, TIM_OC1);

	timer_set_oc_polarity_high(MOTOR_TIM, TIM_OC1N);
	timer_set_oc_idle_state_set(MOTOR_TIM, TIM_OC1N);

	timer_set_oc_value(MOTOR_TIM, TIM_OC1, 100);

	timer_enable_oc_output(MOTOR_TIM, TIM_OC1);
	timer_enable_oc_output(MOTOR_TIM, TIM_OC1N);

	timer_disable_oc_output(MOTOR_TIM, TIM_OC2);
	timer_disable_oc_output(MOTOR_TIM, TIM_OC2N);

	timer_disable_oc_clear(MOTOR_TIM, TIM_OC2);
	timer_enable_oc_preload(MOTOR_TIM, TIM_OC2);
	timer_set_oc_slow_mode(MOTOR_TIM, TIM_OC2);
	timer_set_oc_mode(MOTOR_TIM, TIM_OC2, TIM_OCM_PWM1);

	timer_set_oc_polarity_high(MOTOR_TIM, TIM_OC2);
	timer_set_oc_idle_state_set(MOTOR_TIM, TIM_OC2);

	timer_set_oc_polarity_high(MOTOR_TIM, TIM_OC2N);
	timer_set_oc_idle_state_set(MOTOR_TIM, TIM_OC2N);

	timer_set_oc_value(MOTOR_TIM, TIM_OC2, 100);

	timer_enable_oc_output(MOTOR_TIM, TIM_OC2);
	timer_enable_oc_output(MOTOR_TIM, TIM_OC2N);


	timer_disable_oc_output(MOTOR_TIM, TIM_OC3);
	timer_disable_oc_output(MOTOR_TIM, TIM_OC3N);

	timer_disable_oc_clear(MOTOR_TIM, TIM_OC3);
	timer_enable_oc_preload(MOTOR_TIM, TIM_OC3);
	timer_set_oc_slow_mode(MOTOR_TIM, TIM_OC3);
	timer_set_oc_mode(MOTOR_TIM, TIM_OC3, TIM_OCM_PWM1);

	timer_set_oc_polarity_high(MOTOR_TIM, TIM_OC3);
	timer_set_oc_idle_state_set(MOTOR_TIM, TIM_OC3);

	timer_set_oc_polarity_high(MOTOR_TIM, TIM_OC3N);
	timer_set_oc_idle_state_set(MOTOR_TIM, TIM_OC3N);

	timer_set_oc_value(MOTOR_TIM, TIM_OC3, 100);

	timer_enable_oc_output(MOTOR_TIM, TIM_OC3);
	timer_enable_oc_output(MOTOR_TIM, TIM_OC3N);

	timer_enable_preload(MOTOR_TIM);

	timer_enable_preload_complementry_enable_bits(MOTOR_TIM);


	timer_enable_break_main_output(MOTOR_TIM);

	timer_enable_counter(MOTOR_TIM);

	timer_enable_irq(MOTOR_TIM, TIM_DIER_COMIE);
}

void motor_loop(void){

}



void tim1_trg_tim17_isr(void)
{
	static int step = 0;

	/* Clear the COM trigger interrupt flag. */
	timer_clear_flag(MOTOR_TIM, TIM_SR_COMIF);

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
		timer_set_oc_mode(MOTOR_TIM, TIM_OC1, TIM_OCM_PWM1);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC2, TIM_OCM_FROZEN);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC3, TIM_OCM_FORCE_LOW);

		timer_enable_oc_output(MOTOR_TIM, TIM_OC1);
		timer_disable_oc_output(MOTOR_TIM, TIM_OC1N);

		timer_disable_oc_output(MOTOR_TIM, TIM_OC2);
		timer_disable_oc_output(MOTOR_TIM, TIM_OC2N);

		timer_enable_oc_output(MOTOR_TIM, TIM_OC3);
		timer_enable_oc_output(MOTOR_TIM, TIM_OC3N);

		step++;
		break;
	case 1: /* A HIGH, B PWM LOW, C OFF */
		timer_set_oc_mode(MOTOR_TIM, TIM_OC1, TIM_OCM_FORCE_HIGH);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC2, TIM_OCM_PWM1);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC3, TIM_OCM_FROZEN);

		timer_enable_oc_output(MOTOR_TIM, TIM_OC1);
		timer_enable_oc_output(MOTOR_TIM, TIM_OC1N);

		timer_disable_oc_output(MOTOR_TIM, TIM_OC2);
		timer_enable_oc_output(MOTOR_TIM, TIM_OC2N);

		timer_disable_oc_output(MOTOR_TIM, TIM_OC3);
		timer_disable_oc_output(MOTOR_TIM, TIM_OC3N);

		step++;
		break;
	case 2: /* A OFF, B LOW, C PWM HIGH */
		timer_set_oc_mode(MOTOR_TIM, TIM_OC1, TIM_OCM_FROZEN);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC2, TIM_OCM_FORCE_LOW);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC3, TIM_OCM_PWM1);

		timer_disable_oc_output(MOTOR_TIM, TIM_OC1);
		timer_disable_oc_output(MOTOR_TIM, TIM_OC1N);

		timer_enable_oc_output(MOTOR_TIM, TIM_OC2);
		timer_enable_oc_output(MOTOR_TIM, TIM_OC2N);

		timer_enable_oc_output(MOTOR_TIM, TIM_OC3);
		timer_disable_oc_output(MOTOR_TIM, TIM_OC3N);

		step++;
		break;
	case 3: /* A PWM LOW, B OFF, C HIGH */
		timer_set_oc_mode(MOTOR_TIM, TIM_OC1, TIM_OCM_PWM1);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC2, TIM_OCM_FROZEN);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC3, TIM_OCM_FORCE_HIGH);

		timer_disable_oc_output(MOTOR_TIM, TIM_OC1);
		timer_enable_oc_output(MOTOR_TIM, TIM_OC1N);

		timer_disable_oc_output(MOTOR_TIM, TIM_OC2);
		timer_disable_oc_output(MOTOR_TIM, TIM_OC2N);

		timer_enable_oc_output(MOTOR_TIM, TIM_OC3);
		timer_enable_oc_output(MOTOR_TIM, TIM_OC3N);

		step++;
		break;
	case 4: /* A LOW, B PWM HIGH, C OFF */
		timer_set_oc_mode(MOTOR_TIM, TIM_OC1, TIM_OCM_FORCE_LOW);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC2, TIM_OCM_PWM1);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC3, TIM_OCM_FROZEN);

		timer_enable_oc_output(MOTOR_TIM, TIM_OC1);
		timer_enable_oc_output(MOTOR_TIM, TIM_OC1N);

		timer_enable_oc_output(MOTOR_TIM, TIM_OC2);
		timer_disable_oc_output(MOTOR_TIM, TIM_OC2N);

		timer_disable_oc_output(MOTOR_TIM, TIM_OC3);
		timer_disable_oc_output(MOTOR_TIM, TIM_OC3N);

		step++;
		break;
	case 5: /* A OFF, B HIGH, C PWM LOW */
		timer_set_oc_mode(MOTOR_TIM, TIM_OC1, TIM_OCM_FROZEN);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC2, TIM_OCM_FORCE_HIGH);
		timer_set_oc_mode(MOTOR_TIM, TIM_OC3, TIM_OCM_PWM1);

		timer_disable_oc_output(MOTOR_TIM, TIM_OC1);
		timer_disable_oc_output(MOTOR_TIM, TIM_OC1N);

		timer_enable_oc_output(MOTOR_TIM, TIM_OC2);
		timer_enable_oc_output(MOTOR_TIM, TIM_OC2N);

		timer_disable_oc_output(MOTOR_TIM, TIM_OC3);
		timer_enable_oc_output(MOTOR_TIM, TIM_OC3N);

		step = 0;
		break;
	}
}