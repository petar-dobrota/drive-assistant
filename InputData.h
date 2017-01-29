#pragma once

#include "Pins.h"
#include "OBD.h"
#include "DacConv.h"
#include "Timer.h"
#include "CurieBLE.h"

#define PID_N 2

#ifndef MOCK_OBD

#else
	#include "SPI.h"
	#include "Print.h"
#endif

extern const float GEAR_RATIOS[];

namespace InputData {

	extern int& rpm;
	extern int& speed;
	extern int& engineTemp;
	extern bool clutchDown;
	extern bool clutchPlay;
	extern int lastGear;
	extern bool gearSelected;
	extern bool forceRevMatch;
	extern int throttlePos;
	extern Int64 currentTimeMillis;
	
	bool breakRevMatch();
	int forceRpm();

	bool begin();
	void collect();
};
