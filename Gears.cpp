/*
 * Gears.cpp
 *
 *  Created on: May 15, 2017
 *      Author: petard
 */

#include "Gears.h"

namespace Gears {

const float GEAR_RATIOS[NUM_GEARS] = { 98.4252f, 59.5613f, 40.1856f, 30.6805f, 23.5658f }; // gearRatio=rpm/v

int toRpm(float speed, int gear) {
	if (gear < 1 && gear > NUM_GEARS) {
		return -1;
	}

	return (int) (GEAR_RATIOS[gear - 1] * speed + 0.5f);
}

float toSpeed(int rpm, int gear) {
	if (gear < 1 && gear > NUM_GEARS) {
		return -1;
	}

	return rpm / GEAR_RATIOS[gear - 1];
}

} /* namespace Gears */
