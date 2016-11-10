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
#include <OBD.h>
#include "EngineControl.h"

class FunctionRecorder {

private:
	DataLogger log;

	void logData(int throttlePos, int rpm);
public:
	FunctionRecorder();
	void begin();
	void record(COBD *obd, EngineControl *engine);

};

#endif /* DRIVE_ASSISTANT_FUNCTIONRECORDER_H_ */
