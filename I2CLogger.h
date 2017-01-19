/*
 * I2CLogger.h
 *
 *  Created on: Jan 19, 2017
 *      Author: petard
 */

#ifndef DRIVE_ASSISTANT_I2CLOGGER_H_
#define DRIVE_ASSISTANT_I2CLOGGER_H_

#include "pins.h"
#include "Wire.h"

namespace I2CLogger {
	void logThrottleToRpm();
};

#endif /* DRIVE_ASSISTANT_I2CLOGGER_H_ */
