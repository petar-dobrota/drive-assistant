/*
 * FunctionRecorder.h
 *
 *  Created on: Nov 7, 2016
 *      Author: petard
 */

#ifndef DRIVE_ASSISTANT_FUNCTIONRECORDER_H_
#define DRIVE_ASSISTANT_FUNCTIONRECORDER_H_

#include <DataLogger.h>
#include "Pins.h"
#include "InputData.h"
#include "EngineControl.h"
#include "Wire.h"
#include "DelayTimer.h"
#include "I2CLogger.h"
#include <EEPROM.h>

#define F_REC_MIN_THROTTLE 50
// TODO: probably 205
#define F_REC_MAX_THROTTLE 76

#define F_REC_RESOLUTION 20

#define F_REC_STEP ((float)(((float) F_REC_MAX_THROTTLE - F_REC_MIN_THROTTLE) / (float) F_REC_RESOLUTION))

struct RpmThrottle {
	int rpm;
	unsigned char throttlePos;
};

class FunctionRecorder {

private:

#ifndef DONT_WRITE_SD
//	DataLogger log;
#endif

	int i = 0;
	int direction = 1;

	void logData(int throttlePos, int rpm);
	void stop(EngineControl *engine);
	bool isStoped();

	bool alterMagicSequence(bool inp);
public:
	FunctionRecorder();
	void begin();
	bool recording(EngineControl *engine);

};

#endif /* DRIVE_ASSISTANT_FUNCTIONRECORDER_H_ */
