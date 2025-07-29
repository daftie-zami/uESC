#pragma once 

#include <stdint.h>

#define UART_RX_BUFFER_SIZE 64
#define UART_TX_BUFFER_SIZE 64

typedef struct{
    uint8_t rx_buf[UART_RX_BUFFER_SIZE];
    uint8_t tx_buf[UART_TX_BUFFER_SIZE];
    void * uart_addr;
    uint32_t baudrate;
}uart_dev_t;