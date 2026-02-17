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
void search(Motor& fL, Motor& bL, Motor& fR, Motor& bR);
void moveForward(Motor& fL, Motor& bL, Motor& fR, Motor& bR);
void moveBackward(Motor& fL, Motor& bL, Motor& fR, Motor& bR);
void steerLeft(Motor& fL, Motor& bL, Motor& fR, Motor& bR);
void steerRight(Motor& fL, Motor& bL, Motor& fR, Motor& bR);
void fullStop(Motor& fL, Motor& bL, Motor& fR, Motor& bR);

#endif