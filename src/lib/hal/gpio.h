#pragma once

#include "gpio/hal_gpio.h"

extern void gpio_setup_output(uint32_t port, uint16_t gpios);
extern void gpio_setup_input(uint32_t port, uint16_t gpios);
extern void gpio_setup_input_pullup(uint32_t port, uint16_t gpios);
extern void gpio_setup_input_pulldown(uint32_t port, uint16_t gpios);
#if defined(STM32F1)
extern void gpio_setup_pin_af(uint32_t port, uint16_t pin, uint32_t af, bool is_output);
#else
extern void gpio_setup_pin_af(uint32_t port, uint16_t pin, uint8_t af, bool is_output);
#endif
extern void gpio_setup_pin_analog(uint32_t port, uint16_t pin);
extern void gpio_enable_clock(uint32_t port);