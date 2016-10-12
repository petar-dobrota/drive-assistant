/*
 * OverrevNotifier.h
 *
 *  Created on: Oct 10, 2016
 *      Author: petard
 */

#ifndef OVERREVNOTIFIER_OVERREVNOTIFIER_H_
#define OVERREVNOTIFIER_OVERREVNOTIFIER_H_

#include "InputData.h"

class OverrevNotifier {
private:
	unsigned long lastTick = 0;

	void alarmRinging();
public:
	OverrevNotifier();
	void highRevNotifying(InputData *input);
};

#endif /* OVERREVNOTIFIER_OVERREVNOTIFIER_H_ */
