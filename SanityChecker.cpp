/*
 * SanityChecker.cpp
 *
 *  Created on: May 18, 2017
 *      Author: petard
 */

#include "SanityChecker.h"

namespace SanityChecker {

EngineControl *engine;

void sanityCheck() {
	unsigned long timeNow = millis();

	unsigned long timeSinceLastRead = timeNow - InputData::currentTimeMillis.low();
	if (timeSinceLastRead > MAX_ITERATION_TIME) {
		// something is f**ked up, abort everything

		noInterrupts();
		engine->giveUpControl();

		pinMode(OVERREV_ALARM_PIN, OUTPUT);
		delay(800);
		pinMode(OVERREV_ALARM_PIN, INPUT);

		// die
		while(1);
	}

}

void begin(EngineControl *engine) {
	SanityChecker::engine = engine;

	// sanitycheck every 100ms
	CurieTimerOne.start(SANITY_INTERVAL_MILLIS * 1000, &sanityCheck);  // set timer and callback

	// max time that can pass between reads is SANITY_INTERVAL_MILLIS + MAX_ITERATION_TIME - 1
}



} /* namespace SanityChecker */

