#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <avr/io.h>

//--Clock
#ifndef F_CPU
#define F_CPU 160000000UL

//--UART
#define UART_IN				RX
#define BAUD_RATE 9600

//--Ultra-pins 
#define SONAR_TRIG_PIN 		PB0
#define SONAR_ECHO_PIN 		PB1

//--Motor-pins 
#define MOTOR_ONE_PIN 		PD5
#define MOTOR_TWO_PIN		PD6
#define MOTOR_THREE_PIN		PD7
#define MOTOR_FOUR_PIN		PD8



#endif