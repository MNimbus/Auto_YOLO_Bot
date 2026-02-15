#ifndef MOTORS_HPP
#define MOTORS_HPP

#include <avr/io.h>
#include "config.hpp>

class Motor {
	private:
		volatile uint8_t* port; 				//Pointer to the PORT register
		volatile uint8_t* ddr;					//Pointer to the DDR register
		uint8_t pinA;							//The specific bit for direction 1
		uint8_t pinB;							//The specific bit for direction 2
	
	public:
		Motor(volatile uint8_t* port_reg, volatile uint8_t* ddr_reg, uint8_t pA, uint8_t pB); 	
		// ^^ Constructor
	
	//Methods the motor can take 
	void forward();
	void backward();
	void stop();
};

#endif
	
	