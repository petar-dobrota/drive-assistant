#include "InputData.h"
#include "Pins.h"

const float GEAR_RATIO_TOLERANCE = 0.05f; // gear ratio tolerance when determinating in which gear car is

InputData::InputData() {
	obd = new COBD();
	engineTemp = 0;
	speed = 0;
	rpm = 0;
	clutchDown = false;
	clutchPlay = false;
	gearSelected = false;
	lastGear = 1;
}

bool InputData::begin() {
	obd->begin();
	while (!obd->init())
		;
}

void InputData::gearMonitoring() {

	if (clutchPlay || clutchDown) {
		gearSelected = false;
		return;
	}

	float currRatio = rpm / speed;
	int gear = -1;

	for (int i = 0; i < NUM_GEARS; i++) {
		float maxDelta = GEAR_RATIOS[i] * GEAR_RATIO_TOLERANCE;
		if ((currRatio > GEAR_RATIOS[i] - maxDelta)
				&& (currRatio < GEAR_RATIOS[i] + maxDelta)) {
			gear = i + 1;
			break;
		}
	}

	if (gear != -1) {
		lastGear = gear;
		gearSelected = true;
	} else {
		gearSelected = false;
	}

}

void InputData::collect() {
	int value;

	obd->read(PID_RPM, value);
	rpm = value;

	obd->read(PID_SPEED, value);
	speed = value;

	obd->read(PID_ENGINE_OIL_TEMP, value);
	engineTemp = value;

	clutchDown = digitalRead(CLUTCH_DOWN_PIN);

	// TODO: Read clutch play button
	clutchPlay = clutchDown;

	gearMonitoring();

	// TODO: Add other reads
}
