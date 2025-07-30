#include "hal_rcc.h"
#include "target.h"

#include <libopencm3/stm32/rcc.h>

extern uint32_t rcc_ahb_frequency;
extern uint32_t rcc_apb1_frequency;
extern uint32_t rcc_apb2_frequency;

// -------- STM32F1 --------
#if defined(STM32F1)
static inline void _rcc_clock_setup_f1(void){
#if defined(USE_HSE)
#if (HSE_FREQ_MHZ == 8)
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
#elif (HSE_FREQ_MHZ == 12)
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE12_72MHZ]);
#elif (HSE_FREQ_MHZ == 16)
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE16_72MHZ]);
#elif (HSE_FREQ_MHZ == 25)
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE25_72MHZ]);
#endif
#else
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_72MHZ]);
#endif
}
#endif

// -------- STM32F4 --------
#if defined(STM32F4)
static inline void _rcc_clock_setup_f4(void){
#if defined(USE_HSE)
#if (HSE_FREQ_MHZ == 8)
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_72MHZ]);
#elif (HSE_FREQ_MHZ == 25)
    rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_72MHZ]);
#endif
#else
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_72MHZ]);
#endif
}
#endif

// -------- STM32G0 --------
#if defined(STM32G0)
static inline void _rcc_clock_setup_g0(void){
#if defined(USE_HSE)
#if (HSE_FREQ_MHZ == 8)
    rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_64MHZ]);
#elif (HSE_FREQ_MHZ == 16)
    rcc_clock_setup_hse_3v3(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_64MHZ]);
#endif
#else
    rcc_clock_setup_hsi_3v3(&rcc_hsi_3v3[RCC_CLOCK_3V3_64MHZ]);
#endif
}
#endif

// -------- STM32G4 --------
#if defined(STM32G4)
static inline void _rcc_clock_setup_g4(void){
#if defined(USE_HSE)
#if (HSE_FREQ_MHZ == 8)
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_170MHZ]);
#elif (HSE_FREQ_MHZ == 12)
    rcc_clock_setup_pll(&rcc_hse_12mhz_3v3[RCC_CLOCK_3V3_170MHZ]);
#elif (HSE_FREQ_MHZ == 16)
    rcc_clock_setup_pll(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_170MHZ]);
#elif (HSE_FREQ_MHZ == 24)
    rcc_clock_setup_pll(&rcc_hse_24mhz_3v3[RCC_CLOCK_3V3_170MHZ]);
#endif
#else
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_170MHZ]);
#endif
}
#endif

// -------- Dispatcher --------
void rcc_clock_setup(void){
#if defined(STM32F1)
    _rcc_clock_setup_f1();
#elif defined(STM32F4)
    _rcc_clock_setup_f4();
#elif defined(STM32G0)
    _rcc_clock_setup_g0();
#elif defined(STM32G4)
    _rcc_clock_setup_g4();
#else
#error "Unsupported STM32 family."
#endif
}

uint32_t get_ahb_freq(void){
    return rcc_ahb_frequency;
}

uint32_t get_apb1_freq(void){
    return rcc_apb1_frequency;
}

uint32_t get_apb2_freq(void){
    return rcc_apb2_frequency;
}
