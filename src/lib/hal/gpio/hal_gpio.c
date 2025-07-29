#include "hal/gpio/hal_gpio.h"

#include <libopencm3/stm32/rcc.h>

void gpio_enable_clock(uint32_t port)
{
  switch (port) {
    case GPIOA:
      rcc_periph_clock_enable(RCC_GPIOA);
      break;
    case GPIOB:
      rcc_periph_clock_enable(RCC_GPIOB);
      break;
    case GPIOC:
      rcc_periph_clock_enable(RCC_GPIOC);
      break;
    case GPIOD:
      rcc_periph_clock_enable(RCC_GPIOD);
      break;
#ifdef GPIOE
    case GPIOE:
      rcc_periph_clock_enable(RCC_GPIOE);
      break;
#endif
#ifdef GPIOF
    case GPIOF:
      rcc_periph_clock_enable(RCC_GPIOF);
      break;
#endif
#ifdef GPIOG
    case GPIOG:
      rcc_periph_clock_enable(RCC_GPIOG);
      break;
#endif
    default:
      break;
  };
}

#if defined (STM32F1)
void gpio_setup_output(uint32_t port, uint16_t gpios)
{
  gpio_enable_clock(port);
  gpio_set_mode(port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, gpios);
}

void gpio_setup_input(uint32_t port, uint16_t gpios)
{
  gpio_enable_clock(port);
  gpio_set_mode(port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, gpios);
}

void gpio_setup_input_pullup(uint32_t port, uint16_t gpios)
{
  gpio_enable_clock(port);
  gpio_set(port, gpios);
  gpio_set_mode(port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, gpios);
}

void gpio_setup_input_pulldown(uint32_t port, uint16_t gpios)
{
  gpio_enable_clock(port);
  gpio_clear(port, gpios);
  gpio_set_mode(port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, gpios);
}

void gpio_setup_pin_af(uint32_t port, uint16_t pin, uint32_t af, bool is_output)
{
  gpio_enable_clock(port);
  /* remap alternate function if needed */
  if (af) {
    rcc_periph_clock_enable(RCC_AFIO);
    AFIO_MAPR |= af;
  }
  if (is_output) {
    gpio_set_mode(port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, pin);
  } else {
    gpio_set_mode(port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, pin);
  }
}

void gpio_setup_pin_analog(uint32_t port, uint16_t pin)
{
  gpio_enable_clock(port);
  gpio_set_mode(port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, pin);
}

#else

void gpio_setup_output(uint32_t port, uint16_t gpios)
{
  gpio_enable_clock(port);
  gpio_mode_setup(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, gpios);
}

void gpio_setup_input(uint32_t port, uint16_t gpios)
{
  gpio_enable_clock(port);
  gpio_mode_setup(port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, gpios);
}

void gpio_setup_input_pullup(uint32_t port, uint16_t gpios)
{
  gpio_enable_clock(port);
  gpio_mode_setup(port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, gpios);
}

void gpio_setup_input_pulldown(uint32_t port, uint16_t gpios)
{
  gpio_enable_clock(port);
  gpio_mode_setup(port, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, gpios);
}

void gpio_setup_pin_af(uint32_t port, uint16_t pin, uint8_t af, __attribute__((unused))bool is_output)
{
  gpio_enable_clock(port);
  gpio_set_af(port, af, pin);
  gpio_mode_setup(port, GPIO_MODE_AF, GPIO_PUPD_NONE, pin);
}

void gpio_setup_pin_analog(uint32_t port, uint16_t pin)
{
  gpio_enable_clock(port);
  gpio_mode_setup(port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, pin);
}

#endif