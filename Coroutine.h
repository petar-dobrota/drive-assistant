/*
 * Coroutine.h
 *
 *  Created on: Feb 23, 2017
 *      Author: petard
 */

#ifndef DRIVE_ASSISTANT_COROUTINE_H_
#define DRIVE_ASSISTANT_COROUTINE_H_

class Coroutine {
private:
	long maxExecTimeMillis = -1;

public:
	Coroutine();

	bool loop();
};

#endif /* DRIVE_ASSISTANT_COROUTINE_H_ */
