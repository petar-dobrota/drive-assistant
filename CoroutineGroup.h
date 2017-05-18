/*
 * CoroutineGroup.h
 *
 *  Created on: Feb 23, 2017
 *      Author: petard
 */

#ifndef DRIVE_ASSISTANT_COROUTINEGROUP_H_
#define DRIVE_ASSISTANT_COROUTINEGROUP_H_

#include "Coroutine.h"

class CoroutineGroup {

private:
	int n;
	int coroutineCnt;

	int current;
	Coroutine **coroutines;
public:
	CoroutineGroup(int numRoutines);
	void add(Coroutine *coroutine);

	bool loop();
};

#endif /* DRIVE_ASSISTANT_COROUTINEGROUP_H_ */
