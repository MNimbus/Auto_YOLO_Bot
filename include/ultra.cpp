#include "ultra.hpp"
#include "config.hpp"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void init_ultrasonic(){
	DDRB |= (1<<SONAR_TRIG_PIN); 
	DDRB &= ~(1<<SONAR_ECHO_PIN);
	PORTB &=~(SONAR_TRIG_PIN);
	
	TCCR1A = 0; // Normal mode 
	TCCR1B = (1<< CS11); //Prescaler 8
}
float get_distance(){
	//Send trigger pulse
	PORTB &=~(1<< SONAR_TRIG_PIN); //Clear the trig pin to make sure it is on zero
	_delay_us(2);
	PORTB |= (SONAR_TRIG_PIN); //Pin high to send the pulse
	_delay_us(10);
	PORTB &=~(1 << SONAR_TRIG_PIN); // Pin low 
	
	while (!(PINB &(1 << SONAR_ECHO_PIN))); //Loop to wait for the echo pin to get high input
	uint16_t startTime = TCNT1; //Pull start time from the TCNT1 register
	
	while (PINB & (1 << SONAR_ECHO_PIN)) { //Wait for echo to end (Falling edge)
		if ((TCNT1 -startTime) > 40000) return -1.0; //Timeout if echo stays high too long
	}
	uint16_t endTime = TCNT1;
	
	uint16_t duration = endTime - startTime; //Calculate duration
	
	//Convert to distance (cm)
	//If Prescaler is 8 and clock is 16MHz:
	//Distance = (duration * 0.5us * 0.0343 cm/us) / 2
	return (float)duration * 0.008575;
}