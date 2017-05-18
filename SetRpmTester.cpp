/*
 * SetRpmTester.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: petard
 */

#include "SetRpmTester.h"

namespace SetRpmTester {

bool setRpmTesting(EngineControl *engine) {

	int forceRpm = InputData::forceRpm();
	if (forceRpm < 800 || forceRpm > 7000) {
		return false;
	}

	engine->rpmSetting(forceRpm);

	return true;
}

}
