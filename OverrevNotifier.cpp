/*
 * OverrevNotifier.cpp
 *
 *  Created on: Oct 10, 2016
 *      Author: petard
 */

#include "OverrevNotifier.h"

const int REV_LIMIT = 4000;

OverrevNotifier::OverrevNotifier() {
}

void OverrevNotifier::highRevNotifying() {
	// OUTPUT means ringing
	pinMode(OVERREV_ALARM_PIN, InputData::rpm > REV_LIMIT ? OUTPUT : INPUT);
}
