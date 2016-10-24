/*
 * RevMatcher.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: petard
 */

#include "RevMatcher.h"
#include "Pins.h"
#include "Timer.h"

const Int64 REV_MATCH_MAX_DURATION(2000);
const Int64 REV_MATCH_WAIT_TIME(200); // give clutch plate some time to disengage
const int RPM_SWEET_SPOT = 3000; // always land above 3000 RPM when rev-matching
const float OVERREV_FACTOR = 1.03f; // always overrev for 3%, because RPM will fall a bit while clutch pedal is traveling

RevMatcher::RevMatcher(EngineControl *_engine) {
	this->engine = _engine;
	this->clutchWasDown = false;
}

bool RevMatcher::shouldRevMatch(InputData *input) {

	// clutch must be pressed
	bool shouldRevMatch = true;

	if (input->clutchDown) {
		// rev match preconditions are met

		// check timing now (either is rev-match expired or still shouldn't start)
		Int64 currentTimeMillis = Timer::currentTimeMillis();

		if (!clutchWasDown) {
			// preconditions weren't met in prev. iteration but are now
			// which means we should initiate rev match process
			clutchWasDown = true;
			revmatchStartTime = currentTimeMillis + REV_MATCH_WAIT_TIME;
		}

		// engine must be warmed-up
		shouldRevMatch = shouldRevMatch && (input->engineTemp > 78 && input->engineTemp < 85);

		shouldRevMatch = shouldRevMatch && (revmatchStartTime > currentTimeMillis);
		shouldRevMatch = shouldRevMatch && (revmatchStartTime + REV_MATCH_MAX_DURATION
				< currentTimeMillis);
	} else {
		clutchWasDown = false;
	}

	return shouldRevMatch;
}

bool RevMatcher::revMatching(InputData *input) {

	if (!shouldRevMatch(input)) {
		engine->giveUpControl();
		return false;
	}

	// calculate desired gear (desired gear should be current gear - 1 (if it's not something is probably not right))
	int nextGear = 0; // [1-5]
	float targetRpm = 0;
	for (int i = NUM_GEARS; i > 0; i--) {
		targetRpm = GEAR_RATIOS[i - 1] * input->speed;
		if (targetRpm > RPM_SWEET_SPOT) {
			nextGear = i;
			break;
		}
	}

	if (nextGear == 0) {
		// strange, we failed to find appropriate next gear
		engine->giveUpControl();
		return false;
	} else if (nextGear == 1) {
		// don't rev-match into first, keep second gear
		nextGear = 2;
	}

	// fail safe, make sure you are rev-matching either to same gear or one gear below
	int lastGear = input->lastGear;

	if ((nextGear != lastGear) && (nextGear != lastGear - 1)) {
		// downshifting for more than one gear - don't do that!
		engine->giveUpControl();
		return false;
	}

	// do rev matching if engine isn't in use by somebody else
	engine->rpmSetting(targetRpm * OVERREV_FACTOR);

	// nothing else should be done if already revmatching
	return true;

}
