#ifndef CONFIG_HPP
#define CONFIG_HPP

//--Clock
#ifndef F_CPU
#define F_CPU 160000000UL

//--UART
#define UART_IN				PD0 //RX in from Raspberry Pi
#define BAUD_RATE 			9600
#define UBRR				((F_CPU / (BAUD_RATE * 16UL))-1)

//--Ultra-pins 
#define SONAR_TRIG_PIN 		PB0
#define SONAR_ECHO_PIN 		PB1

//--Motor-pins 
#define M1_IN1 				PD2
#define M1_IN2				PD4
#define M2_IN1				PD7
#define M2_IN2				PB4
#define M3_IN1				PB5
#define M3_IN2				PC0
#define M4_IN1				PC1
#define M4_IN2				PC2

//--PWM Speed pins
#define M1_PWM				PD3
#define M2_PWM				PD6
#define M3_PWM				PD5
#define M4_PWM				PB3

#endif