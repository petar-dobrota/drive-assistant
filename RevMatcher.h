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
#include "Int64.h"

class RevMatcher {

private:
	bool clutchWasDown = false;

	Int64 revmatchStartTime = Int64(0);
	int toGear;


	EngineControl *engine;
	bool shouldRevMatch();
	void breakRevMatch();
	void initiate();
public:

	RevMatcher(EngineControl *e);

	bool revMatching();

};


#endif /* REVMATCHER_H_ */
