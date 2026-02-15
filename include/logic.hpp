#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <avr/io.h>
#include "motors.hpp"

//Define the State machine
enum RobotState {
	SEARCHING,
	TRACK_LEFT,
	TRACK_CENTER,
	TRACK_RIGHT,
	STOP
};

//Different drive functions
void search(Motor& left, Motor& right);
void moveForward(Motor& left, Motor& right);
void moveBackward(Motor& left, Motor& right);
void steerLeft(Motor& left, Motor& right);
void steerRight(Motor& left, Motor& right);
void fullStop(Motor& left, Motor& right);

#endif