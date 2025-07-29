#pragma once

#include <stdint.h>

#include <libopencm3/stm32/gpio.h>

/**
 * Setup one or more pins of the given GPIO port as outputs.
 * @param[in] port
 * @param[in] gpios If multiple pins are to be changed, use logical OR '|' to separate them.
 */
void gpio_setup_output(uint32_t port, uint16_t gpios);

/**
 * Setup one or more pins of the given GPIO port as inputs.
 * @param[in] port
 * @param[in] gpios If multiple pins are to be changed, use logical OR '|' to separate them.
 */
void gpio_setup_input(uint32_t port, uint16_t gpios);

/**
 * Setup one or more pins of the given GPIO port as inputs with pull up resistor enabled.
 * @param[in] port
 * @param[in] gpios If multiple pins are to be changed, use logical OR '|' to separate them.
 */
void gpio_setup_input_pullup(uint32_t port, uint16_t gpios);

/**
 * Setup one or more pins of the given GPIO port as inputs with pull down resistors enabled.
 * @param[in] port
 * @param[in] gpios If multiple pins are to be changed, use logical OR '|' to separate them.
 */
void gpio_setup_input_pulldown(uint32_t port, uint16_t gpios);

/**
 * Setup a gpio for input or output with alternate function.
 */
#if defined(STM32F1)
void gpio_setup_pin_af(uint32_t port, uint16_t pin, uint32_t af, bool is_output);
#else
void gpio_setup_pin_af(uint32_t port, uint16_t pin, uint8_t af,  __attribute__((unused)) bool is_output);
#endif

/**
 * Setup a gpio for analog use.
 */
void gpio_setup_pin_analog(uint32_t port, uint16_t pin);

/**
 * Enable the relevant clock.
 */
void gpio_enable_clock(uint32_t port);