#include <Arduino.h>
#include <Wire.h>
#include <OBD.h>
#include "Int64.h"

#include "pins.h"
#include "InputData.h"

#include "RevMatcher.h"
#include "OverrevNotifier.h"

#include "FunctionRecorder.h"
#include "I2CLogger.h"
#include "SetRpmTester.h"

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

EngineControl engine;
RevMatcher revMatcher(&engine);
OverrevNotifier overrevNotifier;

FunctionRecorder rec;

void beep() {
	pinMode(OVERREV_ALARM_PIN, OUTPUT);
	delay(100);
	pinMode(OVERREV_ALARM_PIN, INPUT);
}

void setup() {
	pinMode(OVERREV_ALARM_PIN, INPUT); // HIGH-Z means alarm OFF
	pinMode(CLUTCH_DOWN_PIN, INPUT);
	pinMode(CLUTCH_PLAY_PIN, INPUT);
	pinMode(FORCE_RM_PIN, INPUT);
	digitalWrite(FORCE_RM_PIN, LOW); // LOW means let original app signal pass
	pinMode(UNUSED1, INPUT);
	pinMode(ENGINE_CTL, OUTPUT);
	pinMode(UNUSED2, INPUT);
	pinMode(UNUSED3, INPUT);
	pinMode(UNUSED4, INPUT);

	Wire.begin();
	InputData::begin();
	rec.begin();

	// signal setup complete
	beep();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

	InputData::collect();
	engine.free();

	overrevNotifier.highRevNotifying();
	
	
	rec.recording(&engine);
	SetRpmTester::setRpmTesting(&engine);
	I2CLogger::logThrottleToRpm();
	//revMatcher.revMatching();

	if (!engine.isBusy()) {
		engine.giveUpControl();
	}
	// TODO: cruiseControling, launchControling

}
