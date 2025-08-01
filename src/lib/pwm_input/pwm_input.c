// TODO

#include "pwm_input.h"
#include "target.h"
#include "pins.h"
#include "hal/rcc.h"
#include "hal/gpio.h"
#include "defs.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <stddef.h>

static volatile uint32_t last_period = 0, last_high = 0;

void pwm_input_init(void) {

    /* 2) PA0 AF1 input */
    gpio_setup_pin_af(GPIOA,GPIO0,GPIO_AF1, 0);

    /* 1) Clock & reset */
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_reset_pulse(RST_TIM2);

    /* 3) Timer base: full 32-bit */
    timer_set_prescaler(TIM2, (get_apb1_freq() / TEN_MHZ) - 1);
    timer_set_period(TIM2, 0xFFFFFFFF);

    /* 4) IC1 = Rising → period */
    timer_ic_set_input(TIM2, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_polarity(TIM2, TIM_IC1, TIM_IC_RISING);
    timer_ic_enable(TIM2, TIM_IC1);

    /* 5) IC2 = Falling → high-time */
    timer_ic_set_input(TIM2, TIM_IC2, TIM_IC_IN_TI1);
    timer_ic_set_polarity(TIM2, TIM_IC2, TIM_IC_FALLING);
    timer_ic_enable(TIM2, TIM_IC2);

    /* 6) Reset-on-TI1 (Slave-Reset Mode) */
    timer_slave_set_mode(TIM2, TIM_SMCR_SMS_RM);
    timer_slave_set_trigger(TIM2, TIM_SMCR_TS_TI1FP1);

    /* 7) IRQ: CC1 & CC2 under TIM2_IRQ */
    timer_enable_irq(TIM2, TIM_DIER_CC1IE | TIM_DIER_CC2IE);
    nvic_enable_irq(NVIC_TIM2_IRQ);

    /* 8) Start counter */
    timer_enable_counter(TIM2);
}

void tim2_isr(void) {
    uint32_t sr = TIM_SR(TIM2);
    if (sr & TIM_SR_CC1IF) {
        last_period = TIM_CCR1(TIM2);
        timer_clear_flag(TIM2, TIM_SR_CC1IF);
    }
    if (sr & TIM_SR_CC2IF) {
        last_high = TIM_CCR2(TIM2);
        timer_clear_flag(TIM2, TIM_SR_CC2IF);
    }
}
