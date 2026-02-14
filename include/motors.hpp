#ifndef MOTORS_HPP
#define MOTORS_HPP

#include <avr/io.h>
#include "Config.hpp>

class Motor {
	private:
		uint8_t ddr_pin;
		uint8_t port_pin;
	public:
	Motor(uint8_t pin); //Constructor: Sets up the pins 
	
	//Methods the motor can take 
	void forward();
	void backward();
	void stop();
	void setSpeed(uint8_t speed);
};

#endif
	
	