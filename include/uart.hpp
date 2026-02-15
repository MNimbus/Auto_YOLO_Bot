#ifndef UART_HPP
#define UART_HPP 

#include <stdint.h>

void UART_init(uint16_t ubrr); //initialize UART witht a specific UBRR value
unsigned char UART_receive(void); //receive a single byte 

#endif