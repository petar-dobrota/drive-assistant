/*
 * FunctionRecorder.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: petard
 */

#include "FunctionRecorder.h"

const int n = 30;

FunctionRecorder::FunctionRecorder() {

}

void FunctionRecorder::begin() {
	log.begin(13);
}

void FunctionRecorder::record(COBD *obd, EngineControl *engine) {

	int minT = 39;
	int maxT = 100; // TODO: Probably: 205;

	float step = (float) (maxT - minT) / n;
	int i = 0;
	int direction = 1;
	while(true) {

		int throttle = minT + (int) (step * i + 0.5f);
		engine->setThrottlePos(throttle);
		delay(500);
		int rpm;
		obd->readPID(PID_RPM, rpm);
		logData(throttle, rpm);

		i += direction;
		if (i > n) {
			// reached one end, go back
			direction = direction * -1;
		} else if (i < 0) {
			// reached beginning, do one iteration for now -> break
			break;
		}
	}
}
