/*
 * SetRpmTester.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: petard
 */

#include "SetRpmTester.h"

namespace SetRpmTester {

bool setRpmTesting(EngineControl *engine) {

	if (engine->isBusy()) {
		return false;
	}

	int forceRpm = InputData::forceRpm();
	if (forceRpm < 800 || forceRpm > 7500) {
		engine->giveUpControl();
		engine->free();
		return false;
	}

	engine->rpmSetting(forceRpm);

	return true;
}

}
