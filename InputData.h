#pragma once

#include "OBD.h"

extern const float GEAR_RATIOS[];

class InputData {
	
private:

	COBD *obd;
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
