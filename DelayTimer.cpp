/*
 * DelayTimer.cpp
 *
 *  Created on: Jan 19, 2017
 *      Author: petard
 */

#include "DelayTimer.h"

DelayTimer::DelayTimer() {
	this->delayEnd = Int64(-1);
}

bool DelayTimer::delaying(int delay) {
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
