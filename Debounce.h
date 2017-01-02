/*
 * Debounce.h
 *
 *  Created on: Oct 20, 2016
 *      Author: petard
 */
#ifndef DRIVE_ASSISTANT_DEBOUNCE_H_
#define DRIVE_ASSISTANT_DEBOUNCE_H_

#include "Int64.h"

class Debounce {

private:
	const unsigned pin;

	bool oldValue;

	bool transitionValue = false;
	Int64 transitionStartTime = 0;

	bool transitionState = false;

public:
	Debounce(unsigned pin);

	bool readAsync();
	bool readBlocking();

};

#endif /* DRIVE_ASSISTANT_DEBOUNCE_H_ */
