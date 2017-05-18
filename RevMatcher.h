/*
 * RevMatcher.h
 *
 *  Created on: Aug 25, 2016
 *      Author: petard
 */

#ifndef REVMATCHER_H_
#define REVMATCHER_H_

#include "EngineControl.h"
#include "InputData.h"
#include "Pins.h"
#include "Gears.h"

class RevMatcher {

private:
	enum State {
		idle,
		prepared,
		inProgress,
		canceled,
		disabled
	};

	int calcNextGear();
	bool rpmSweetSpot(int rpm);
	void transitionToCanceled();
	void rmInProgress();

	State state;
	Int64 revmatchStartTime = Int64(0);
	int toGear;


	EngineControl *engine;

public:

	RevMatcher(EngineControl *e);

	bool revMatching();

};


#endif /* REVMATCHER_H_ */
