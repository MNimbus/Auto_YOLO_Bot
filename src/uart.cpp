#include <avr/io.h>
#include "uart.hpp"
#include "config.hpp"

void uart_init(uint16_t ubrr) {
	//UBRR0H(High) & UBRR0L(Low) baud rate registers
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
	
	UCSR0B = (1 << RXEN0); // Tells the Atmega to start listening to the RX pin
	
	UCSROC = (1 << UCSZ01) | (1<< UCSZ00); //Sets the frame format 
}

unsigned char uart_receive(void) {
	while (!(UCSR0A & (1 << RXC0)))//Wait for data to be received
	//^ polling loop,  looks at the RXC0 bit 
	//if the bit is 0, the CPU stays in this loop because no data has arrived
	return UDR0;
}