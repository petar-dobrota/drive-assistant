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
	while (!log.begin(SD_CS));
	delay(500);
	File f = log.startLogging();
	f.print("time\tapp\trpm\n");
	f.close();
}

void FunctionRecorder::logData(int throttlePos, int rpm) {
	// time app rpm

	File f = log.startLogging();
	f.print(millis());
	f.print('\t');
	f.print(throttlePos);
	f.print('\t');
	f.print(rpm);
	f.print('\n');
	f.close();
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

bool FunctionRecorder::recording(InputData *in, EngineControl *engine) {

	if (isStoped()) return false;

	int throttle = F_REC_MIN_THROTTLE + (int) (F_REC_STEP * i + 0.5f);
	engine->setThrottlePos(throttle);

	if (delaying(500)) return true;

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
