#include "pins.h"
#include "target.h"
#include "system.h"
#include "hal/gpio.h"
#include "hal/rcc.h"
#include "hal/systick.h"
#include "defs.h"
#include "pwm_input/pwm_input.h"
#include "pwm_output.h"

#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/usart.h>
#include <stddef.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/dwt.h>
#include <stdint.h>
#include <stddef.h>

float throttle;
float duty;

#if defined (CONSOLE_UART)
void console_init(void){

    gpio_setup_pin_af(CONSOLE_TX_GPIO_Port,CONSOLE_TX_Pin,GPIO_AF6, 1);
	gpio_setup_pin_af(CONSOLE_RX_GPIO_Port,CONSOLE_RX_Pin,GPIO_AF6, 1);

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

uint32_t benchmark;

void super_func(uint32_t baud){

    gpio_setup_pin_af(PWM_INPUT_GPIO_Port,PWM_INPUT_Pin,GPIO_AF1, 1);

    // Clock enable & reset
    rcc_periph_clock_enable(RCC_DMA1);
    rcc_periph_clock_enable(PWM_INPUT_TIM_RCC);
    rcc_periph_reset_pulse(PWM_INPUT_TIM_RST);

    timer_set_mode(PWM_INPUT_TIM, TIM_CR1_CKD_CK_INT,TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    timer_set_prescaler(PWM_INPUT_TIM, 0);

    timer_set_period(PWM_INPUT_TIM, (get_apb1_freq() / baud) - 1);

    timer_disable_oc_output(PWM_INPUT_TIM, TIM_OC1);
    timer_set_oc_mode(PWM_INPUT_TIM, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_value(PWM_INPUT_TIM, TIM_OC1, 0);
    timer_enable_oc_output(PWM_INPUT_TIM, TIM_OC1);

    timer_enable_counter(PWM_INPUT_TIM);
}

int main(void) {
	system_init();
	rcc_clock_setup();
	systick_init();
    dwt_enable_cycle_counter();
#if defined (LED_GPIO_Port)
	gpio_setup_output(LED_GPIO_Port,LED_Pin);
#endif
	motor_pwm_init(MOTOR_PWM_FREQ);

#if defined (USE_PWM_INPUT)
    // pwm_input_init();
#endif
	motor_set_duty(200, 400, 600); 
    super_func();
 #if defined (KEY_GPIO_Port)
	gpio_setup_input(KEY_GPIO_Port,KEY_Pin);
 #endif

	for (;;) {
        static uint32_t start;
        start = dwt_read_cycle_counter();
#if defined (LED_GPIO_Port)
	gpio_toggle(LED_GPIO_Port,LED_Pin);
#endif
        if(KEY_Pin != gpio_get(KEY_GPIO_Port,KEY_Pin)){
		    system_reset();
        }
        delay_ms(50);
        benchmark = dwt_read_cycle_counter() - start;
	}
	return 0;
}
