#include "motor.hpp"
#include <avr/io.h>
#include <util/delay.h>
#include "config.hpp"

Motor::Motor(volatile uint8_t* port_reg, volatile uint8_t* ddr_reg, uint8_t pA, uint8_t pB); {
	port = port_reg;
	ddr = ddr_reg;
	pinA = pA;
	pinB = pB;
	
	*ddr |= (1<< pinA) | (1 << pinB);
}

void Motor::forward() {
	*port |= (1 << pinA); //pinA HIGH
	*port &= ~(1<< pinB); //pinB LOW
}

void Motor::backward() {
	*port |= (1 << pinB); //pinB HIGH
	*port &= ~(1<< pinA); //pinA LOW
}
void Motor::stop() {
	*port &= ~(1 << pinA); //pinA LOW
	*port &= ~(1<< pinB); //pinB LOW
}
void Motor::setSpeed(uint8_t dutyCycle) {
	//dutyCycle would be 255 for 100% speed, 127 for 50% speed
	*pwmPort = dutyCycle; //This sets the Compare register for the PWM timer