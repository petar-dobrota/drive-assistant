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
#include "Timer.h"

class RevMatcher {

private:
	bool clutchWasDown = false;
	Int64 revmatchStartTime = Int64(0);

	EngineControl *engine;

	bool shouldRevMatch(InputData *input);

public:

	RevMatcher(EngineControl *e);

	bool revMatching(InputData *input);

};


#endif /* REVMATCHER_H_ */
