/*
 * EngineControl.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: petard
 */
#include <Arduino.h>
#include "EngineControl.h"
#include "Wire.h"
#include "Pins.h"

const int MAX_THROTTLE_POS = 800;
const int MIN_THROTTLE_POS = 36;

#define DAC_ADDRESS 0x90
#define DAC_WRITE_CMD 0x40

void EngineControl::setThrottlePos(int pos) {
	 Wire.beginTransmission(DAC_ADDRESS);
	 Wire.write(DAC_WRITE_CMD);
	 Wire.write(pos);
	 Wire.endTransmission();
	 digitalWrite(ENGINE_CTL, HIGH);
}

EngineControl::EngineControl(InputData *input, RpmToThrottleFunction *rpmToThrottle) {
	this->input = input;
	this->rpmToThrottle = rpmToThrottle;
}

void EngineControl::rpmSetting(float desiredRpm) {
	static float closeRange = 0.08f;

	float diff = (float) (input->rpm - desiredRpm) / (input->rpm + desiredRpm);
	if (diff < 0) {
		diff = -diff;
	}

	int out;
	if (diff < closeRange) {
		out = rpmToThrottle->getThrottle(desiredRpm);
	} else if (input->rpm < desiredRpm) {
		out = MAX_THROTTLE_POS;
	} else {
		out = MIN_THROTTLE_POS;
	}

	setThrottlePos(out + 0.5f);
}

void EngineControl::giveUpControl() {
	digitalWrite(ENGINE_CTL, LOW);
}
