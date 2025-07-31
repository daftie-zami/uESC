#include "pins.h"
#include "target.h"
#include "system.h"
#include "hal/gpio.h"
#include "hal/rcc.h"
#include "hal/systick.h"
#include "defs.h"
#include "pwm_input/pwm_input.h"

#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <stddef.h>
#include <libopencm3/cm3/nvic.h>
#include <stdint.h>
#include <stddef.h>

float throttle;
float duty;

void motor_pwm_init(uint32_t pwm_freq) {

    gpio_setup_pin_af(PWM_U_HIGH_GPIO_Port,PWM_U_HIGH_Pin,GPIO_AF6, 1);
	gpio_setup_pin_af(PWM_V_HIGH_GPIO_Port,PWM_V_HIGH_Pin,GPIO_AF6, 1);
	gpio_setup_pin_af(PWM_W_HIGH_GPIO_Port,PWM_W_HIGH_Pin,GPIO_AF6, 1);

	gpio_setup_pin_af(PWM_U_LOW_GPIO_Port,PWM_U_LOW_Pin,GPIO_AF4, 1);
	gpio_setup_pin_af(PWM_V_LOW_GPIO_Port,PWM_V_LOW_Pin,GPIO_AF6, 1);
	gpio_setup_pin_af(PWM_W_LOW_GPIO_Port,PWM_W_LOW_Pin,GPIO_AF4, 1);

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

    // Deadtime ve break settings
    timer_set_deadtime(MOTOR_PWM_TIM, MOTOR_PWM_DT);
    timer_set_enabled_off_state_in_idle_mode(MOTOR_PWM_TIM);
    timer_set_enabled_off_state_in_run_mode(MOTOR_PWM_TIM);
    timer_disable_break(MOTOR_PWM_TIM);
    timer_set_break_polarity_high(MOTOR_PWM_TIM);
    timer_disable_break_automatic_output(MOTOR_PWM_TIM);
    timer_set_break_lock(MOTOR_PWM_TIM, TIM_BDTR_LOCK_OFF);

    // CH1, CH2, CH3 and N outputs
    const enum tim_oc_id ch[] = { TIM_OC1, TIM_OC2, TIM_OC3 };
    for (int i = 0; i < 3; i++) {
        enum tim_oc_id main = ch[i];
        enum tim_oc_id comp = main + 1; // TIM_OCxN complementary output

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

        timer_set_oc_value(MOTOR_PWM_TIM, main, 0);

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

#if defined (CONSOLE_UART)

void console_init(void){

    gpio_setup_pin_af(CONSOLE_TX_GPIO_Port,CONSOLE_TX_Pin,GPIO_AF6, 7);
	gpio_setup_pin_af(CONSOLE_RX_GPIO_Port,CONSOLE_RX_Pin,GPIO_AF6, 7);

    rcc_periph_clock_enable(CONSOLE_UART_RCC);

	/* Setup UART parameters. */
	usart_set_baudrate(CONSOLE_UART, CONSOLE_BAUD);
	usart_set_databits(CONSOLE_UART, 8);
	usart_set_stopbits(CONSOLE_UART, USART_STOPBITS_1);
	usart_set_mode(CONSOLE_UART, USART_MODE_TX_RX);
	usart_set_parity(CONSOLE_UART, USART_PARITY_NONE);
	usart_set_flow_control(CONSOLE_UART, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(CONSOLE_UART);
}

void console_write(uint8_t *data, uint16_t size){
    for(uint16_t i = 0; i < size; i++){
        usart_send_blocking(CONSOLE_UART, data[i]);
    }
}
#endif

int main(void) {

	system_init();
	rcc_clock_setup();
	systick_init();
	delay_ms(500);
#if defined (LED_GPIO_Port)
	gpio_setup_output(LED_GPIO_Port,LED_Pin);
#endif
	motor_pwm_init(MOTOR_PWM_FREQ);

#if defined (USE_PWM_INPUT)
    pwm_input_init();
#endif
	motor_set_duty(200, 400, 600); 

 #if defined (KEY_GPIO_Port)
	gpio_setup_input(KEY_GPIO_Port,KEY_Pin);
 #endif

	for (;;) {
#if defined (LED_GPIO_Port)
	gpio_toggle(LED_GPIO_Port,LED_Pin);
#endif
        if(KEY_Pin != gpio_get(KEY_GPIO_Port,KEY_Pin)){
		    system_reset();
        }
        delay_ms(50);
	}
	return 0;
}
