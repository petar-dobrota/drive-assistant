/*
 * SanityChecker.h
 *
 *  Created on: May 18, 2017
 *      Author: petard
 */

#ifndef DRIVE_ASSISTANT_SANITYCHECKER_H_
#define DRIVE_ASSISTANT_SANITYCHECKER_H_

#include "pins.h"
#include "CurieTimerOne.h"
#include "InputData.h"
#include "EngineControl.h"
#include <Arduino.h>

#define SANITY_INTERVAL_MILLIS 100
#define MAX_ITERATION_TIME 200

namespace SanityChecker {

	void begin(EngineControl *engine);

} /* namespace SanityChecker */

#endif /* DRIVE_ASSISTANT_SANITYCHECKER_H_ */
