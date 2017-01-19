/*
 * EngineControl.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: petard
 */
#include "EngineControl.h"

const int MAX_THROTTLE_POS = 205;
const int MIN_THROTTLE_POS = 39;

void EngineControl::setThrottlePos(int pos) {
	DacConv::setOutput(pos);
	digitalWrite(ENGINE_CTL, HIGH);

#ifdef MOCK_OBD
	 Serial.print("m");
	 Serial.println(pos);
#endif
}

void EngineControl::rpmSetting(float desiredRpm) {
	static float closeRange = 0.08f;

	float diff = (float) (InputData::rpm - desiredRpm) / (InputData::rpm + desiredRpm);
	if (diff < 0) {
		diff = -diff;
	}

	int out;
	if (diff < closeRange) {
		out = RpmToThrottleFunction::getThrottle(desiredRpm);
	} else if (InputData::rpm < desiredRpm) {
		out = MAX_THROTTLE_POS;
	} else {
		out = MIN_THROTTLE_POS;
	}

	setThrottlePos(out + 0.5f);
}

void EngineControl::giveUpControl() {
	digitalWrite(ENGINE_CTL, LOW);

#ifdef MOCK_OBD
	Serial.println("m-");
#endif
}
