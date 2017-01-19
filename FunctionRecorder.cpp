/*
 * FunctionRecorder.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: petard
 */

#include "FunctionRecorder.h"

FunctionRecorder::FunctionRecorder() {

}

void FunctionRecorder::begin() {

}

void FunctionRecorder::logData(int throttlePos, int rpm) {

#ifndef DONT_WRITE_LOG

	Wire.beginTransmission(8);

	Wire.print(millis());
	Wire.print('\t');
	Wire.print(throttlePos);
	Wire.print('\t');
	Wire.print(rpm);
	Wire.print('\n');

	Wire.endTransmission();
#endif

	static int currAddr = 0;

	RpmThrottle point;
	point.rpm = rpm;
	point.throttlePos = throttlePos;

	EEPROM.put(currAddr, point);
	currAddr+=sizeof(RpmThrottle);

}

void FunctionRecorder::stop(EngineControl *engine) {
	direction = 0;
	engine->giveUpControl();
}

bool FunctionRecorder::isStoped() {
	return direction == 0;
}

bool FunctionRecorder::alterMagicSequence(bool inp) {

	unsigned long timeNow = millis();

	static int step = 0;
	static unsigned long lastTick = 0;

	// wait a bit to let things settle after step change.
	if (timeNow < lastTick + 50) {
		return false;
	}

	bool oddStep = (step % 2) == 1;

	if (inp == oddStep) {
		if (timeNow > lastTick + 2500) {
			// step taking too long - reset sequence
			step = 0;
			return false;
		}
	} else {
		if (timeNow > lastTick + 200) {
			step++;
			lastTick = timeNow;
		}
	}

	if (step == 6) {
		step = 0;
		return true;
	} else {
		return false;
	}

}

bool FunctionRecorder::recording(EngineControl *engine) {

	static bool magicSequenceCompleted = false;

	if (!magicSequenceCompleted) {
		if (!alterMagicSequence(InputData::breakRevMatch())) {
			return false;
		}

		if (InputData::clutchDown) {
			magicSequenceCompleted = true;
		} else {
			return false;
		}
	}

	if (isStoped())
		return false;

	int throttle = F_REC_MIN_THROTTLE + (int) (F_REC_STEP * i + 0.5f);
	engine->setThrottlePos(throttle);

	if (InputData::breakRevMatch()) {
		stop(engine);
		return false;
	}

	static DelayTimer d;
	if (d.delaying(1400, InputData::currentTimeMillis)) {
		return true;
	}

	logData(throttle, InputData::rpm);

	i += direction;
	if (i > F_REC_RESOLUTION) {
		// reached one end, go back
		direction = direction * -1;
	} else if (i < 0) {
		// reached beginning, do one iteration for now -> break
		stop(engine);
		return false;
	}

}
