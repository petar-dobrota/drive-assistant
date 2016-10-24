#pragma once

#define MOCK_OBD

#ifndef MOCK_OBD
	#include "OBD.h"
#else
	#include "Print.h"
#endif

extern const float GEAR_RATIOS[];

class InputData {
	
private:

#ifndef MOCK_OBD
	COBD obd;
#endif
 
	void gearMonitoring();

public:
	
	float rpm;
	int speed;
	int engineTemp;
	bool clutchDown;
	bool clutchPlay;
	int lastGear;
	bool gearSelected;

	InputData();
	
	bool begin();
	
	void collect();
};
