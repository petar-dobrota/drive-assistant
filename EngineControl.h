/*
 * EngineControl.h
 *
 *  Created on: Aug 25, 2016
 *      Author: petard
 */

#ifndef ENGINECONTROL_H_
#define ENGINECONTROL_H_

#include "Pins.h"
#include "InputData.h"
#include "RpmToThrottleFunction.h"
#include "DacConv.h"

class EngineControl {
private:

	bool busy = false;

public:
	// TODO: Move to private
	void setThrottlePos(int pos);

	void rpmSetting(float rpm);
	// returns throttle pedal control to driver
	void giveUpControl();

	void free();
	bool isBusy();
};

#endif /* ENGINECONTROL_H_ */
