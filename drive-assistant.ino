#include <Arduino.h>
#include <Wire.h>
#include <OBD.h>
#include "Int64.h"
#include "Timer.h"

#include "pins.h"
#include "InputData.h"

#include "RpmToThrottleFunction.h"
#include "RevMatcher.h"
#include "OverrevNotifier.h"

#include "FunctionRecorder.h"

// obd adapter pinout:
// rx - white
// tx - green

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTANTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const float GEAR_RATIOS[5] =
		{ 98.4252f, 59.5613f, 40.1856f, 30.6805f, 23.5658f }; // gearRatio=rpm/v

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Timer time;
InputData input;
RpmToThrottleFunction rpmToThrottle;
EngineControl engine(&input, &rpmToThrottle);
RevMatcher revMatcher(&engine);
OverrevNotifier overrevNotifier;

FunctionRecorder rec;

void beep() {

	bool on = true;

	for (int i = 0; i < 250; i++) {

		on = ((i % 3) != 0);
		digitalWrite(OVERREV_ALARM_PIN, !on);
		delayMicroseconds(1000);
	}
	digitalWrite(OVERREV_ALARM_PIN, HIGH);

}

void setup() {
	pinMode(OVERREV_ALARM_PIN, OUTPUT);
	pinMode(CLUTCH_DOWN_PIN, INPUT);
	pinMode(CLUTCH_PLAY_PIN, INPUT);
	pinMode(FORCE_RM_PIN, INPUT);
	pinMode(UNUSED1, INPUT);
	pinMode(ENGINE_CTL, OUTPUT);
	pinMode(UNUSED2, INPUT);
	pinMode(UNUSED3, INPUT);
	pinMode(SD_CS, OUTPUT);

	digitalWrite(OVERREV_ALARM_PIN, HIGH);

	input.begin();
	Wire.begin();

	rec.begin();

	beep();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {

	input.collect();

	overrevNotifier.highRevNotifying(&input);

	if (rec.recording(&input, &engine)) {
		return;
	}

#ifndef DONT_WRITE_LOG
	Wire.beginTransmission(8);

	Wire.print(millis());
	Wire.print('\t');
	Wire.print(input.throttlePos);
	Wire.print('\t');
	Wire.print(input.rpm);
	Wire.print('\n');

	Wire.endTransmission();
#endif
	if (revMatcher.revMatching(&input)) {
		return;
	}

	// TODO: cruiseControling, launchControling

}
