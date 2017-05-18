#include "RevMatcher.h"
#include "InputData.h"

#ifdef MOCK_OBD
const Int64 REV_MATCH_MAX_DURATION(1000000);
#else
const Int64 REV_MATCH_MAX_DURATION(2000);
#endif

const Int64 REV_MATCH_START_DELAY(200); // give clutch plate some time to disengage
const int RPM_SWEET_SPOT = 3300; // always land above 3000 RPM when rev-matching
const int MAX_TARGET_RPM = 6000;

RevMatcher::RevMatcher(EngineControl* e) {
	this->engine = e;
	this->state = State::idle;
}

int RevMatcher::calcNextGear() {
	for (int i = NUM_GEARS; i > 0; i--) {
		int potentialRpm = Gears::toRpm(InputData::speed, i);
		if (rpmSweetSpot(potentialRpm)) {
			return i;
		}
	}
	return -1;
}

bool RevMatcher::rpmSweetSpot(int rpm) {
	return rpm >= RPM_SWEET_SPOT && rpm < MAX_TARGET_RPM;
}

void RevMatcher::transitionToCanceled() {
	this->engine->giveUpControl();
	this->state = State::canceled;
}

void RevMatcher::rmInProgress() {
	int targetRpm = Gears::toRpm(InputData::speed, this->toGear);

	if (!rpmSweetSpot(targetRpm)) {
		// RPM not in range -> cancel
		transitionToCanceled();
	} else if (InputData::currentTimeMillis >= this->revmatchStartTime + REV_MATCH_MAX_DURATION) {
		// Max RM duration breached -> abort
		transitionToCanceled();
	} else if (!InputData::clutchDown || !InputData::clutchPlay) {
		// clutch let-off -> RM is completed
		this->engine->giveUpControl();
		this->state = State::idle;
	} else if (InputData::breakRevMatch()) {
		transitionToCanceled();
	} else {
		// do RM
		engine->rpmSetting(targetRpm);
	}
}

bool RevMatcher::revMatching() {
	switch (this->state) {
	case idle:
		if (InputData::clutchDown) {
			// transition to "prepared"
			this->toGear = calcNextGear();
			this->revmatchStartTime = InputData::currentTimeMillis + REV_MATCH_START_DELAY;
			this->state = State::prepared;
		}
		break;
	case prepared:
		if (!InputData::clutchDown || !InputData::clutchPlay) {
			// clutch isn't engaged anymore -> break
			this->state = idle;
		} else if (InputData::currentTimeMillis > this->revmatchStartTime) {
			// transition to "inProgress"
			this->state = inProgress;
		}
		break;
	case inProgress:
		rmInProgress();
		break;
	case canceled:
		engine->giveUpControl();
		if (!InputData::clutchDown && !InputData::clutchPlay) {
			this->state = State::idle;
		}
		break;
	case disabled:
		engine->giveUpControl();
		// never leave disabled state
		break;
	}
}
