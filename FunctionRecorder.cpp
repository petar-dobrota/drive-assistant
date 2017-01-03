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

	Wire.beginTransmission(8);

	Wire.print(millis());
	Wire.print('\t');
	Wire.print(throttlePos);
	Wire.print('\t');
	Wire.print(rpm);
	Wire.print('\n');

	Wire.endTransmission();
}

bool FunctionRecorder::delaying(long delay) {
	static Int64 delayEnd = Int64(-1);

	Int64 currTime = Timer::currentTimeMillis();
	if (delayEnd < 0) {
		delayEnd = currTime + Int64(delay);
	}

	if (currTime >= delayEnd) {
		delayEnd = Int64(-1);
		return false;
	} else {
		return true;
	}
}

void FunctionRecorder::stop(EngineControl *engine) {
	direction = 0;
	engine->giveUpControl();
}

bool FunctionRecorder::isStoped() {
	return direction == 0;
}

bool FunctionRecorder::alterMagicSequence(bool inp) {
	static int step = 0;
	static unsigned long lastTick = 0;

	unsigned long timeNow = millis();
	bool oddStep = (step % 2) == 1;

	if (inp == oddStep) {
		if (timeNow > lastTick + 1500) {
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

bool FunctionRecorder::recording(InputData *in, EngineControl *engine) {

	static bool magicSequenceCompleted = false;

	if (!magicSequenceCompleted) {
		if (!alterMagicSequence(in->breakRevMatch())) {
			return false;
		}

		if (in->clutchDown) {
			magicSequenceCompleted = true;
		} else {
			return false;
		}
	}

	if (isStoped())
		return false;

	int throttle = F_REC_MIN_THROTTLE + (int) (F_REC_STEP * i + 0.5f);
	engine->setThrottlePos(throttle);

	if (delaying(500))
		return true;

	in->collect();
	logData(throttle, in->rpm);

	if (in->breakRevMatch()) {
		stop(engine);
		return false;
	}

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
