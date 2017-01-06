#include "InputData.h"

const float GEAR_RATIO_TOLERANCE = 0.05f; // gear ratio tolerance when determinating in which gear car is

InputData::InputData() {
	engineTemp = 0;
	speed = 0;
	rpm = 0;
	clutchDown = false;
	clutchPlay = false;
	gearSelected = false;
	forceRevMatch = false;
	lastGear = 1;
	throttlePos = 0;
	pids[0] = PID_RPM;
	pids[1] = PID_SPEED;
	pids[2] = PID_COOLANT_TEMP;
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

	float currRatio = rpm / (float) speed;
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

int readObd(COBD &obd, int pid) {
	bool ok;
	int value;

	for (int i = 0; i < 7; i++) {
		ok = obd.readPID(pid, value);
		if (ok) break;
	}

	return value;
}
void InputData::collect() {

#ifndef MOCK_OBD

	this->rpm = readObd(this->obd, PID_RPM);
	this->speed = readObd(this->obd, PID_SPEED);
	this->engineTemp = 80;

	// TODO: Uncomment code below when piece of shit OBD driver is being fixed
/*
	bool success = false;

	for (int i = 0; i < 10 && !success; i++) {
		int pidReads = obd.readPID(pids, PID_N, p);
		success = pidReads == PID_N;
	}
*/

	clutchDown = false; //digitalRead(CLUTCH_DOWN_PIN);

	// TODO: Read clutch play button
	clutchPlay = clutchDown;

	// TODO: Add other reads (ENGINE TEMP)

#else
	Serial.println("r");

	rpm = readInt();
	speed = readInt();
	engineTemp = readInt();
	clutchPlay = readBool();
	clutchDown = readBool();
	engineTemp = readInt();
#endif

	// read throttle pos from I2C ADC
	throttlePos = DacConv::getInput();

#ifdef MOCK_OBD
	 Serial.print("tp");
	 Serial.println(throttlePos);
#endif

	gearMonitoring();
}

bool InputData::breakRevMatch() {
	return this->throttlePos > 130;
}
