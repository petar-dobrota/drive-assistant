#pragma once

#include "Pins.h"
#include "OBD2UART.h"
#include "DacConv.h"
#include "Timer.h"

#ifdef ENABLE_BLE
	#include "CurieBLE.h"
#endif

#define PID_N 2

#ifdef MOCK_OBD
	#include "SPI.h"
	#include "Print.h"
#else
#endif

extern const float GEAR_RATIOS[];

namespace InputData {

	extern int& rpm;
	extern int& speed;
	extern int engineTemp;
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
