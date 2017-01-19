/*
 * RevMatcher.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: petard
 */

#include "RevMatcher.h"

#ifdef MOCK_OBD
	const Int64 REV_MATCH_MAX_DURATION(1000000);
#else
	const Int64 REV_MATCH_MAX_DURATION(2000);
#endif

const Int64 REV_MATCH_START_DELAY(200); // give clutch plate some time to disengage
const int RPM_SWEET_SPOT = 3000; // always land above 3000 RPM when rev-matching
const float OVERREV_FACTOR = 1.03f; // always overrev for 3%, because RPM will fall a bit while clutch pedal is traveling

RevMatcher::RevMatcher(EngineControl *_engine) {
	this->engine = _engine;
	this->clutchWasDown = false;
	this->toGear = 0;
}

void RevMatcher::breakRevMatch() {
	this->toGear = 0;
	this->engine->giveUpControl();
}

bool RevMatcher::shouldRevMatch() {

	bool shouldRevMatch = false;

	if (InputData::clutchDown) {
		// rev match preconditions are met

		if (!clutchWasDown || InputData::forceRevMatch) {
			initiate();
			clutchWasDown = true;
		}

		shouldRevMatch = !InputData::breakRevMatch();
		shouldRevMatch = shouldRevMatch && toGear > 0;

		// engine must be warmed-up
		shouldRevMatch = shouldRevMatch && (InputData::engineTemp > 78 && InputData::engineTemp < 85);

		// if some of check above failed, means we shouldn't rev match in this clutch press cycle
		if (!shouldRevMatch) {
			breakRevMatch();
		}

		// check only whether is right time to RevMatch, if it's not, no need for breakRevMatch
		shouldRevMatch = shouldRevMatch && (revmatchStartTime < InputData::currentTimeMillis);
		shouldRevMatch = shouldRevMatch &&
				(revmatchStartTime + REV_MATCH_MAX_DURATION > InputData::currentTimeMillis);

	} else {
		clutchWasDown = false;
		breakRevMatch();
	}

	return shouldRevMatch;
}

void RevMatcher::initiate() {
	// preconditions weren't met in prev. iteration but are now
	// which means we should initiate rev match process
	revmatchStartTime = InputData::currentTimeMillis + REV_MATCH_START_DELAY;

	// calculate desired gear (desired gear should be current gear - 1 (if it's not something is probably not right))
	toGear = 0; // [1-5]
	for (int i = NUM_GEARS; i > 0; i--) {
		float targetRpm = GEAR_RATIOS[i - 1] * InputData::speed;
		if (targetRpm > RPM_SWEET_SPOT) {
			toGear = i;
			break;
		}
	}

	if (toGear == 1) {
		// don't rev-match into first, give up
		toGear = 0;
	}

	int lastGear = InputData::lastGear;

	// fail safe, make sure you are rev-matching to gear below
	// unless it's force RM, force RM will cause RM anyway
	if (!InputData::forceRevMatch && (toGear != lastGear - 1)) {
		// downshifting for more than one gear - don't do that!
		toGear = 0;
	}

	// TODO: Check how reliable is input.lastGear. If it reliable try to shift to lastGear-1(check rpm will be between 3k and 5k)
	// If it's not reliable, sack checking last gear at all
}

bool RevMatcher::revMatching() {

	if (!shouldRevMatch()) {
		engine->giveUpControl();
		return false;
	}

	float targetRpm = GEAR_RATIOS[this->toGear - 1] * InputData::speed;

	// do rev matching if engine isn't in use by somebody else
	engine->rpmSetting(targetRpm * OVERREV_FACTOR);

	// nothing else should be done if already revmatching
	return true;

}
