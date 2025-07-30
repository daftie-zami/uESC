#include "pins.h"
#include "target.h"
#include "system.h"
#include "hal/gpio.h"
#include "hal/rcc.h"
#include "hal/systick.h"
#include "defs.h"

#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <stdint.h>
#include <stddef.h>

float throttle;
float duty;

void motor_pwm_init(uint32_t pwm_freq) {

    gpio_setup_pin_af(PHASE_U_GPIO_HIGH_GPIO_Port,PHASE_U_GPIO_HIGH_Pin,GPIO_AF6, NULL);
	gpio_setup_pin_af(PHASE_V_GPIO_HIGH_GPIO_Port,PHASE_V_GPIO_HIGH_Pin,GPIO_AF6, NULL);
	gpio_setup_pin_af(PHASE_W_GPIO_HIGH_GPIO_Port,PHASE_W_GPIO_HIGH_Pin,GPIO_AF6, NULL);

	gpio_setup_pin_af(PHASE_U_GPIO_LOW_GPIO_Port,PHASE_U_GPIO_LOW_Pin,GPIO_AF6, NULL);
	gpio_setup_pin_af(PHASE_V_GPIO_LOW_GPIO_Port,PHASE_V_GPIO_LOW_Pin,GPIO_AF6, NULL);
	gpio_setup_pin_af(PHASE_W_GPIO_LOW_GPIO_Port,PHASE_W_GPIO_LOW_Pin,GPIO_AF6, NULL);

    // Clock enable & reset
    rcc_periph_clock_enable(MOTOR_PWM_TIM_RCC);
    rcc_periph_reset_pulse(MOTOR_PWM_TIM_RST);

    // Timer base settings 
    timer_set_prescaler(MOTOR_PWM_TIM, 0);
    timer_set_mode(MOTOR_PWM_TIM, TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_repetition_counter(MOTOR_PWM_TIM, 0);
    timer_enable_preload(MOTOR_PWM_TIM);
    timer_continuous_mode(MOTOR_PWM_TIM);
    
    // Set ARR value
    timer_set_period(MOTOR_PWM_TIM, (get_apb2_freq() / pwm_freq) - 1);

    // Deadtime ve break ayarları
    timer_set_deadtime(MOTOR_PWM_TIM, MOTOR_PWM_DT);
    timer_set_enabled_off_state_in_idle_mode(MOTOR_PWM_TIM);
    timer_set_enabled_off_state_in_run_mode(MOTOR_PWM_TIM);
    timer_disable_break(MOTOR_PWM_TIM);
    timer_set_break_polarity_high(MOTOR_PWM_TIM);
    timer_disable_break_automatic_output(MOTOR_PWM_TIM);
    timer_set_break_lock(MOTOR_PWM_TIM, TIM_BDTR_LOCK_OFF);

    // CH1, CH2, CH3 ve N outputs
    const enum tim_oc_id ch[] = { TIM_OC1, TIM_OC2, TIM_OC3 };
    for (int i = 0; i < 3; i++) {
        enum tim_oc_id main = ch[i];
        enum tim_oc_id comp = main + 1; // TIM_OCxN

        timer_disable_oc_output(MOTOR_PWM_TIM, main);
        timer_disable_oc_output(MOTOR_PWM_TIM, comp);

        timer_disable_oc_clear(MOTOR_PWM_TIM, main);
        timer_enable_oc_preload(MOTOR_PWM_TIM, main);
        timer_set_oc_slow_mode(MOTOR_PWM_TIM, main);
        timer_set_oc_mode(MOTOR_PWM_TIM, main, TIM_OCM_PWM1);

        timer_set_oc_polarity_high(MOTOR_PWM_TIM, main);
        timer_set_oc_idle_state_set(MOTOR_PWM_TIM, main);

        timer_set_oc_polarity_high(MOTOR_PWM_TIM, comp);
        timer_set_oc_idle_state_set(MOTOR_PWM_TIM, comp);

        timer_set_oc_value(MOTOR_PWM_TIM, main, 100);

        timer_enable_oc_output(MOTOR_PWM_TIM, main);
        timer_enable_oc_output(MOTOR_PWM_TIM, comp);
    }

    timer_enable_preload_complementry_enable_bits(MOTOR_PWM_TIM);
    timer_enable_break_main_output(MOTOR_PWM_TIM);
    timer_enable_counter(MOTOR_PWM_TIM);
}

void motor_set_duty(uint16_t u, uint16_t v, uint16_t w) {
    timer_set_oc_value(MOTOR_PWM_TIM, TIM_OC1, u);
    timer_set_oc_value(MOTOR_PWM_TIM, TIM_OC2, v);
    timer_set_oc_value(MOTOR_PWM_TIM, TIM_OC3, w);
}


int main(void) {

	system_init();
	rcc_clock_setup();
	systick_init();
	delay_ms(500);
	gpio_setup_output(LED_GPIO_Port,LED_Pin);

	motor_pwm_init(10000);
	motor_set_duty(200, 400, 600); 

 #if defined (KEY_GPIO_Port)
	gpio_setup_input_pulldown(KEY_GPIO_Port,KEY_Pin);
 #endif

	for (;;) {
		gpio_toggle(LED_GPIO_Port,LED_Pin);
		delay_ms(50);
	}
	return 0;
}
