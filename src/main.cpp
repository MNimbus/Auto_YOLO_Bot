#include <avr/io.h>
#include <util/delay.h>
#include "motors.hpp"
#include "logic.hpp"
#include "config.hpp"
#include "uart.hpp"
#include "ultra.hpp"

Motor fL(&PORTD, &DDRB, M1_IN1, M1_IN2);
Motor bL(&PORTD, &DDRB, M2_IN1, &PORTB, &DDRB, M2_IN2);
Motor fR(&PORTD, &DDRB, M3_IN1, &PORTC, &DDRC, M3_IN2);
Motor bR(&PORTC, &DDRC, M4_IN1, M4_IN2);

RobotState currentState = 	SEARCHING;

int main(void) {
	UART_init();
	
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