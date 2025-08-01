// #include "dshot.h"
// #include <libopencm3/cm3/cortex.h>
// #include <libopencm3/stm32/rcc.h>
// #include <libopencm3/stm32/dma.h>
// #include <libopencm3/stm32/timer.h>

// // ---- send state ----
// static uint16_t dma_send_buffer[16];
// static uint32_t send_tim;
// static uint8_t send_dma_ch;
// static uint32_t send_high_one;
// static uint32_t send_high_zero;
// static uint32_t send_period;
// static uint32_t bb_port;
// static uint16_t bb_pin;

// // ---- input state ----
// static uint16_t dma_input_buffer[32];
// static uint32_t input_tim;
// static uint8_t input_dma_ch;
// static volatile bool input_ready = false;
// static uint16_t input_throttle = 0;


// static uint8_t dshot_checksum(uint16_t value){
//     uint8_t c = 0;
//     for (uint8_t i = 0; i < 3; i++) {
//         c ^= (value >> (i * 4)) & 0xF;
//     }
//     return c & 0xF;
// }


// // ---- DMA send ----
// void dshot_init_dma(uint32_t tim, uint8_t channel, uint32_t dma, uint8_t dma_channel, uint32_t tim_clk_hz, uint32_t dshot_speed_khz){
//     send_tim = tim; send_dma_ch = dma_channel;
//     send_period = tim_clk_hz / (dshot_speed_khz * 1000U);
//     send_high_one = (send_period*2U)/3U;
//     send_high_zero = send_period/3U;
//     // timer PWM config
//     timer_set_mode(tim, channel>>2, TIM_CR1_CKD_CKD_DIV1, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
//     timer_set_oc_mode(tim,channel,TIM_OCM_PWM1);
//     timer_enable_oc_preload(tim,channel);
//     timer_set_oc_value(tim,channel,0);
//     timer_enable_counter(tim);
//     // dma clock
//     rcc_periph_clock_enable(RCC_DMA1);
// }
// void dshot_send_dma(uint16_t throttle){
//     uint16_t packet = (throttle<<1)|dshot_checksum(throttle);
//     for(int8_t i=15;i>=0;i--) dma_send_buffer[15-i]=(((packet>>i)&1U)?send_high_one:send_high_zero);
//     dma_stream_reset((send_tim==TIM1?DMA1:DMA2),send_dma_ch);
//     dma_set_peripheral_address((send_tim==TIM1?DMA1:DMA2),send_dma_ch,(uint32_t)&TIM_CCR1(send_tim));
//     dma_set_memory_address((send_tim==TIM1?DMA1:DMA2),send_dma_ch,(uint32_t)dma_send_buffer);
//     dma_set_number_of_data((send_tim==TIM1?DMA1:DMA2),send_dma_ch,16);
//     dma_set_peripheral_size((send_tim==TIM1?DMA1:DMA2),send_dma_ch,DMA_CCR_PSIZE_16BIT);
//     dma_set_memory_size((send_tim==TIM1?DMA1:DMA2),send_dma_ch,DMA_CCR_MSIZE_16BIT);
//     dma_enable_memory_increment_mode((send_tim==TIM1?DMA1:DMA2),send_dma_ch);
//     dma_set_priority((send_tim==TIM1?DMA1:DMA2),send_dma_ch,DMA_CCR_PL_HIGH);
//     dma_set_read_from_memory((send_tim==TIM1?DMA1:DMA2),send_dma_ch);
//     dma_enable_channel((send_tim==TIM1?DMA1:DMA2),send_dma_ch);
//     timer_enable_dma_request(send_tim,TIM_DIER_UDE);
// }

// // ---- DMA input ----
// void dshot_init_input(uint32_t tim, uint8_t channel, uint32_t dma, uint8_t dma_channel, uint32_t tim_clk_hz){
//     input_tim=tim; input_dma_ch=dma_channel;
//     // timer capture mode
//     timer_set_mode(tim,channel>>2,TIM_CR1_CKD_CKD_DIV1,TIM_CR1_CMS_EDGE,TIM_CR1_DIR_UP);
//     timer_ic_set_input(tim,channel,TIM_IC_IN_TI1);
//     timer_set_ic_polarity(tim,channel,TIM_IC_POLARITY_BOTH);
//     timer_enable_counter(tim);
//     // dma init
//     rcc_periph_clock_enable((dma==DMA1)?RCC_DMA1:RCC_DMA2);
//     dma_stream_reset((tim==TIM1?DMA1:DMA2),dma_channel);
//     dma_set_peripheral_address((tim==TIM1?DMA1:DMA2),dma_channel,(uint32_t)&TIM_CCR1(tim));
//     dma_set_memory_address((tim==TIM1?DMA1:DMA2),dma_channel,(uint32_t)dma_input_buffer);
//     dma_set_number_of_data((tim==TIM1?DMA1:DMA2),dma_channel,32);
//     dma_set_peripheral_size((tim==TIM1?DMA1:DMA2),dma_channel,DMA_CCR_PSIZE_16BIT);
//     dma_set_memory_size((tim==TIM1?DMA1:DMA2),dma_channel,DMA_CCR_MSIZE_16BIT);
//     dma_enable_memory_increment_mode((tim==TIM1?DMA1:DMA2),dma_channel);
//     dma_set_priority((tim==TIM1?DMA1:DMA2),dma_channel,DMA_CCR_PL_HIGH);
//     dma_set_read_from_peripheral((tim==TIM1?DMA1:DMA2),dma_channel);
//     dma_enable_circular_mode((tim==TIM1?DMA1:DMA2),dma_channel);
//     dma_enable_channel((tim==TIM1?DMA1:DMA2),dma_channel);
//     timer_enable_dma_request(tim,TIM_DIER_CC1DE);
// }

// // ISR for DMA input
// void dma1_channel1_isr(void){
//     if(dma_get_interrupt_flag(DMA1,input_dma_ch,DMA_TCIF)){
//         dma_clear_interrupt_flags(DMA1,input_dma_ch,DMA_TCIF);
//         // decode
//         uint16_t diffs[16];
//         for(int i=0;i<16;i++) diffs[i]=dma_input_buffer[2*i+1]-dma_input_buffer[2*i];
//         uint16_t half=(dma_input_buffer[31]-dma_input_buffer[0])>>5;
//         uint16_t val=0;
//         for(int i=0;i<11;i++){
//             val|=(diffs[i]>half)?(1<< (10-i)):0;
//         }
//         input_throttle=val;
//         input_ready=true;
//     }
// }

// bool dshot_available(void){return input_ready;}
// uint16_t dshot_read(void){input_ready=false;return input_throttle;}
