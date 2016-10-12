/*
 * OverrevNotifier.cpp
 *
 *  Created on: Oct 10, 2016
 *      Author: petard
 */

#include <Arduino.h>
#include "OverrevNotifier.h"

#include "Pins.h"

const int REV_LIMIT = 2500;
const unsigned long ALARM_HALF_T_MICROS = (500000 / 1500); // 150Hz

OverrevNotifier::OverrevNotifier() {
}

void OverrevNotifier::alarmRinging() {
	unsigned long now = micros();

	if (now < lastTick) {
		// patch timer overflow
		lastTick = now;
	}

	if (lastTick + ALARM_HALF_T_MICROS <= now) {
		// whole periode passed
		digitalWrite(OVERREV_ALARM_PIN, !digitalRead(OVERREV_ALARM_PIN));
		lastTick = now;
	}
}

void OverrevNotifier::highRevNotifying(InputData *input) {

	  if (input->rpm > REV_LIMIT) {
	     alarmRinging();
	  } else {
	    // alarm off
	    digitalWrite(OVERREV_ALARM_PIN, LOW);
	  }

	  // high revs notify doesn't interfere with other co-routines

}
