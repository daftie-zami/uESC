
#include <stdint.h>

void motor_pwm_init(uint32_t pwm_freq);
void motor_set_duty(uint16_t u, uint16_t v, uint16_t w);