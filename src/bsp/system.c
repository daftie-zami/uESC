#include "system.h"
#include "target.h"

#include <stdint.h>

extern uint32_t __ccmram_start__;
extern uint32_t __ccmram_end__;

#if defined(USE_CCM_RAM)
static inline void zero_ccmram(void);
#endif

void system_init(void){
#if defined(USE_CCM_RAM)
    zero_ccmram();
#endif
}

#if defined(USE_CCM_RAM)
static inline void zero_ccmram(void) {
    uint32_t *dst = &__ccmram_start__;
    while (dst < &__ccmram_end__) {
        *dst++ = 0;
    }
}
#endif