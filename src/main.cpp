#include <avr/io.h>
#include <util/delay.h>
#include "motors.hpp"
#include "logic.hpp"
#include "config.hpp"
#include "uart.hpp"
#include "ultra.hpp"

//Create objects for each motor, define pins as in the schematic
//(Data reg, direction reg, Pin 1, Pin 2, PWM reg)

Motor fL(&PORTD, &DDRB, M1_IN1, M1_IN2, &OCR2B);
Motor bL(&PORTD, &DDRB, M2_IN1, M2_IN2, &OCR0A);
Motor fR(&PORTB, &DDRB, M3_IN1, M3_IN2, &OCR0B);
Motor bR(&PORTC, &DDRC, M4_IN1, M4_IN2, &OCR2A);

RobotState currentState = 	SEARCHING;

int main(void) {
	UART_init(UBRR);
	
	while(1) {
		
		int distance = get_distance(); //Get distance from Ultra-sensor
		
		if(distance < 30) { //If distance less than 30 cm 
			currentState = STOP; // Stop
		}
		else {
			//Path is clear: listen for directions from Raspberry pi
				if(UCSR0A & (1 << RXC0)) {
					char message = UART_receive(); //Receive the direction over UART
					if (message == 'L') currentState = TRACK_LEFT;
					else if (message == 'C') currentState = TRACK_CENTER;
					else if (message == 'R') currentState = TRACK_RIGHT;
					else if (message == 'N') currentState = SEARCHING;
			}
		}
		//Switch triggers specific motor logic based on the state decided by UART
		switch(currentState) {
			case SEARCHING:
				search(fL, bL, fR, bR);
				break;
			
			case TRACK_CENTER:
				moveForward(fL, bL, fR, bR);
				break;
			
			case TRACK_RIGHT:
				steerRight(fL, bL, fR, bR);
				break;
			
			case TRACK_LEFT:
				steerLeft(fL, bL, fR, bR);
				break;
			case STOP:
				fullStop(fL, bL, fR, bR);
				break;
		}
	}
}