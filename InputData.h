#pragma once

#include "Pins.h"
#include "OBD.h"
#include "DacConv.h"

#define PID_N 3

#ifndef MOCK_OBD

#else
	#include "SPI.h"
	#include "Print.h"
#endif

extern const float GEAR_RATIOS[];

class InputData {
	
private:

#ifndef MOCK_OBD
	COBD obd;
#endif
 
	void gearMonitoring();

	int p[PID_N];
	byte pids[PID_N];
public:
	
	int& rpm = p[0];
	int& speed = p[1];
	int& engineTemp = p[2];
	bool clutchDown;
	bool clutchPlay;
	int lastGear;
	bool gearSelected;
	bool forceRevMatch;
	bool breakRevMatch();
	int throttlePos;
	InputData();
	
	bool begin();
	
	void collect();
};
