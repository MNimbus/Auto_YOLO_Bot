#include <avr/io.h>
#include "uart.hpp"
#include "config.hpp"

void UART_init(uint16_t ubrr) {
											//UBRR0H(High) & UBRR0L(Low) baud rate registers
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;
	
	UCSR0B = (1 << RXEN0);					// Tells the Atmega to start listening to the RX pin
	
	UCSR0C = (1 << UCSZ01) | (1<< UCSZ00);	//Sets the frame format 
}

unsigned char UART_receive(void) {
	while (!(UCSR0A & (1 << RXC0))){
		}									//Wait for data to be received
											//polling loop,  looks at the RXC0 bit 
											//if the bit is 0, the CPU stays in this loop because no data has arrived
	return UDR0;							// Wait for the Receive Complete (RXC0) flag, then return the 8-bit data from the UDR0 register.
}
//Below code is for non blocking UART function, current above code will be stuck in UART loop until message is received

////Change the logic so it returns 0 (or a special character) if nothing is there
//unsigned char UART_receive_non_blocking(void) {
	//if (!(UCSR0A & (1 << RXC0))) {
		//return 0; // No data arrived, return 0 and move on immediately
	//}
	//return UDR0; // Data is there! Grab it and return it.
//}