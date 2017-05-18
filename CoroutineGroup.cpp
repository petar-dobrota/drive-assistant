/*
 * CoroutineGroup.cpp
 *
 *  Created on: Feb 23, 2017
 *      Author: petard
 */

#include "CoroutineGroup.h"

CoroutineGroup::CoroutineGroup(int numRoutines) {
	n = numRoutines;
	coroutines = new Coroutine*[n];
	coroutineCnt = 0;
	current = -1;
}

void CoroutineGroup::add(Coroutine* coroutine) {
	if (coroutineCnt < n) {
		coroutines[coroutineCnt++] = coroutine;
	}
}

bool CoroutineGroup::loop() {
	if (current != -1) {
		bool more = coroutines[current]->loop();
		if (more) {
			return true;
		} else {
			current = -1;
			return false;
		}
	}

	for (int i = 0; i < this->coroutineCnt; i++) {
		bool takeLock = coroutines[current]->loop();
		if (takeLock) {
			current = i;
			return true;
		}
	}

	current = -1;
	return false;
}
