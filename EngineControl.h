/*
 * EngineControl.h
 *
 *  Created on: Aug 25, 2016
 *      Author: petard
 */

#ifndef ENGINECONTROL_H_
#define ENGINECONTROL_H_

#include "InputData.h"
#include "RpmToThrottleFunction.h"

class EngineControl {
private:

	InputData *input;
	RpmToThrottleFunction *rpmToThrottle;

	void setThrottlePos(int pos);

public:
	EngineControl(InputData *input, RpmToThrottleFunction *rpmToThrottle);
	void rpmSetting(float rpm);
	// returns throttle pedal control to driver
	void giveUpControl();
};

#endif /* ENGINECONTROL_H_ */
