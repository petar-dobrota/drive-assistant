/*
 * Debounce.cpp
 *
 *  Created on: Oct 20, 2016
 *      Author: petard
 */

#include "Debounce.h"
#include <Arduino.h>
#include "Timer.h"

static const Int64 SAFE_DELAY = 5;

Debounce::Debounce(unsigned _pin) : pin(_pin){
	oldValue = digitalRead(pin);
}

bool Debounce::readAsync() {
	bool currValue = digitalRead(this->pin);
	Int64 currentTime = Timer::currentTimeMillis();

	if (!transitionState) {
		// passive
		if (currValue != oldValue) {
			// start transition
			transitionStartTime = currentTime;
			transitionValue = currValue;
			transitionState = true;
		}
	} else {
		// transition (keep transition, end transition(revert, change)
		if (currValue == transitionValue) {
			// keep transition or commit
			if (currentTime > transitionStartTime + SAFE_DELAY) {
				// commit
				digitalWrite(pin, currValue);
				transitionState = false;
			} else {
				// keep transition
			}
		} else {
			// value still unstable, revert
			transitionState = false;
		}

	}

	oldValue = currValue;
}

bool Debounce::readBlocking() {
	bool retVal;

	int staleCounter = 0;

	while(staleCounter < 3) {
		retVal = readAsync();
		if (transitionState) {
			staleCounter = 0;
		} else {
			staleCounter++;
		}
	}

	return retVal;
}
