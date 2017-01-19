/*
 * I2CLogger.cpp
 *
 *  Created on: Jan 19, 2017
 *      Author: petard
 */

#include "I2CLogger.h"

namespace I2CLogger {

void logThrottleToRpm () {
#ifndef DONT_WRITE_LOG
	Wire.beginTransmission(8);

	Wire.print(millis());
	Wire.print('\t');
	Wire.print(InputData::throttlePos);
	Wire.print('\t');
	Wire.print(InputData::rpm);
	Wire.print('\n');

	Wire.endTransmission();
#endif
}

} /* namespace DacConv */
