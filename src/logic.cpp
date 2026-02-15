#include "logic.hpp"
#include <util/delay.h>
#include "config.hpp"

void search(Motor& fL, Motor& bL, Motor& fR, Motor& bR) {
	//Left Speed 
	fL.setSpeed();
	bL.setSpeed();
	//Right Speed 
	fR.setSpeed();
	bR.setSpeed();
		
	//Left wheels
	fL.forward();
	bL.forward();
	//Right Wheels
	fR.backward();
	bR.backward();
	
	_delay_ms(100);
	
	fullStop(fL, bL, fR, bR);
	}
	
void moveForward(Motor& fL, Motor& bL, Motor& fR, Motor& bR) {
	//Left Speed 
	fL.setSpeed();
	bL.setSpeed();
	//Right Speed 
	fR.setSpeed();
	bR.setSpeed();
	
	//Left Wheels
	fL.forward();
	bL.forward();
	//Right Wheels
	fR.forward();
	bR.forward();
}
	
void moveBackward(Motor& fL, Motor& bL, Motor& fR, Motor& bR) {
	//Left Speed 
	fL.setSpeed();
	bL.setSpeed();
	//Right Speed 
	fR.setSpeed();
	bR.setSpeed();
	
	//Left Wheels
	fL.backward();
	bL.backward();
	//Right Wheels
	fR.backward();
	bR.backward();
	}
void steerLeft(Motor& fL, Motor& bL, Motor& fR, Motor& bR) {
	//Left Speed 
	fL.setSpeed();
	bL.setSpeed();
	//Right Speed 
	fR.setSpeed();
	bR.setSpeed();
	
	//Left Wheels
	fL.backward();
	bL.backward();
	//Right Wheels
	fR.backward();
	bR.backward();
	}
void steerRight(Motor& fL, Motor& bL, Motor& fR, Motor& bR) {
	//Left Speed 
	fL.setSpeed();
	bL.setSpeed();
	//Right Speed 
	fR.setSpeed();
	bR.setSpeed();
	
	//Left Wheels
	fL.backward();
	bL.backward();
	//Right Wheels
	fR.backward();
	bR.backward();	
}
void fullStop(Motor& fL, Motor& bL, Motor& fR, Motor& bR) {
	//Left Wheels
	fL.stop();
	bL.stop();
	//Right Wheels
	fR.stop();
	bR.stop();
}