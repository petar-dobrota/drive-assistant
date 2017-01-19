#include "InputData.h"

namespace InputData {

const float GEAR_RATIO_TOLERANCE = 0.05f; // gear ratio tolerance when determinating in which gear car is

#ifndef MOCK_OBD
	COBD obd;
#else
	int readInt() {
		static unsigned char buff[13];
		Serial.readBytes(buff, 2);
		return (int) buff[0] + (int) buff[1] * 256;
	}

	bool readBool() {
		return readInt() != 0;
	}
#endif

	int p[PID_N];
	byte pids[PID_N];

	int& rpm = p[0];
	int& speed = p[1];
	int& engineTemp = p[2];
	bool clutchDown = false;
	bool clutchPlay = false;
	int lastGear = 0;
	bool gearSelected = false;
	bool forceRevMatch = false;
	int throttlePos = 0;

	Int64 currentTimeMillis;
bool begin() {

	pids[0] = PID_RPM;
	pids[1] = PID_SPEED;
	pids[2] = PID_COOLANT_TEMP;
#ifndef MOCK_OBD
	obd.begin();
	while (!obd.init());
#else
	Serial.begin(57600);
	Serial.println("hello");
#endif

	return true;
}

void gearMonitoring() {

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

void collect() {

#ifndef MOCK_OBD

	// TODO: Uncomment code below when piece of shit OBD driver is being fixed

	bool success = false;

	for (int i = 0; i < 10 && !success; i++) {
		int pidReads = obd.readPID(pids, PID_N, p);
		success = pidReads == PID_N;
	}

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

	currentTimeMillis = Timer::currentTimeMillis();
}

bool breakRevMatch() {
	return throttlePos > 130;
}

}
