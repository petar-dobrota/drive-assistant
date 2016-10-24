#include "InputData.h"
#include "Pins.h"

const float GEAR_RATIO_TOLERANCE = 0.05f; // gear ratio tolerance when determinating in which gear car is

InputData::InputData() {
	engineTemp = 0;
	speed = 0;
	rpm = 0;
	clutchDown = false;
	clutchPlay = false;
	gearSelected = false;
	lastGear = 1;
}

bool InputData::begin() {

#ifndef MOCK_OBD
	obd.begin();
	while (!obd.init());
#else
	Serial.begin(57600);
	Serial.println("hello");
#endif

	return true;
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

#ifdef MOCK_OBD
	int readInt() {
		static unsigned char buff[13];
		Serial.readBytes(buff, 2);
		return (int) buff[0] + (int) buff[1] * 256;
	}

	bool readBool() {
		return readInt() != 0;
	}

#endif

void InputData::collect() {

#ifndef MOCK_OBD
	int value;
	obd.readPID(PID_RPM, value);
	rpm = value;

	obd.readPID(PID_SPEED, value);
	speed = value;

	obd.readPID(PID_ENGINE_OIL_TEMP, value);
	engineTemp = value;

	clutchDown = false; //digitalRead(CLUTCH_DOWN_PIN);

	// TODO: Read clutch play button
	clutchPlay = clutchDown;

	// TODO: Add other reads

#else
	Serial.print("r");

	rpm = readInt();
	speed = readInt();
	engineTemp = readInt();
	clutchPlay = readBool();
	clutchDown = readBool();

#endif

	gearMonitoring();
}
