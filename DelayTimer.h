/*
 * DelayTimer.h
 *
 *  Created on: Jan 19, 2017
 *      Author: petard
 */

#ifndef DRIVE_ASSISTANT_DELAYTIMER_H_
#define DRIVE_ASSISTANT_DELAYTIMER_H_

#include "Timer.h"

class DelayTimer {
private:
	Int64 delayEnd;
public:
	DelayTimer();
	bool delaying(int delay);
};

#endif /* DRIVE_ASSISTANT_DELAYTIMER_H_ */
