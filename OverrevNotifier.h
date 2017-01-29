/*
 * OverrevNotifier.h
 *
 *  Created on: Oct 10, 2016
 *      Author: petard
 */

#ifndef OVERREVNOTIFIER_OVERREVNOTIFIER_H_
#define OVERREVNOTIFIER_OVERREVNOTIFIER_H_

#include <Arduino.h>
#include "Pins.h"
#include "InputData.h"

class OverrevNotifier {
private:

public:
	OverrevNotifier();
	void highRevNotifying();
};

#endif /* OVERREVNOTIFIER_OVERREVNOTIFIER_H_ */
