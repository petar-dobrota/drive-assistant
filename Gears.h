/*
 * Gears.h
 *
 *  Created on: May 15, 2017
 *      Author: petard
 */

#ifndef DRIVE_ASSISTANT_GEARS_H_
#define DRIVE_ASSISTANT_GEARS_H_

#define NUM_GEARS 5

namespace Gears {

	int toRpm(float speed, int gear);
	float toSpeed(int rpm, int gear);
}

#endif /* DRIVE_ASSISTANT_GEARS_H_ */
