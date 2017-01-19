/*
 * OverrevNotifier.cpp
 *
 *  Created on: Oct 10, 2016
 *      Author: petard
 */

#include <Arduino.h>
#include "OverrevNotifier.h"

const int REV_LIMIT = 4000;
const unsigned long ALARM_HALF_T_MICROS = (500000 / 1500); // 1500Hz (500000 / 1500);

OverrevNotifier::OverrevNotifier() {
}

void OverrevNotifier::highRevNotifying() {
	// LOW means ringing
	digitalWrite(OVERREV_ALARM_PIN, InputData::rpm > REV_LIMIT ? LOW : HIGH);

}
