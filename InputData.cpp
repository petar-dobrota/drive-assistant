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

#ifdef ENABLE_BLE
BLEPeripheral blePeripheral;
BLEService bleService("19B10010-E8F2-537E-4F6C-D104768A1266");

BLECharCharacteristic bleForceRpm("19B10010-E8F2-537E-4F6C-D104768A1266", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic bleLastIterationMillis("19B10010-E8F2-537E-4F6C-D104768A1267", BLERead);
BLEUnsignedIntCharacteristic bleAvgIterationMillis("19B10010-E8F2-537E-4F6C-D104768A1268", BLERead);
BLEUnsignedIntCharacteristic bleRpm("19B10010-E8F2-537E-4F6C-D104768A1269", BLERead);
#endif

int pidVal[PID_N];
byte pids[PID_N];

int& rpm = pidVal[0];
int& speed = pidVal[1];
int engineTemp = 80;
bool clutchDown = false;
bool clutchPlay = false;
bool gearSelected = false;
bool forceRevMatch = false;
int throttlePos = 0;

Int64 currentTimeMillis;

bool begin() {

	pids[0] = PID_RPM;
	pids[1] = PID_SPEED;

#ifdef ENABLE_BLE
	blePeripheral.setLocalName("Drive-Assistant");
	blePeripheral.setAdvertisedServiceUuid(bleService.uuid());

	blePeripheral.addAttribute(bleService);
	blePeripheral.addAttribute(bleForceRpm);
	blePeripheral.addAttribute(bleLastIterationMillis);
	blePeripheral.addAttribute(bleAvgIterationMillis);
	blePeripheral.addAttribute(bleRpm);

	bleForceRpm.setValue(-1);

	blePeripheral.begin();
#endif

#ifndef MOCK_OBD
	obd.begin();
	while (!obd.init());
#else
	Serial.begin(57600);
	Serial.println("hello");
#endif

	return true;
}

void bleCollect() {
#ifdef ENABLE_BLE
	// time update and collect execution time data
	static float numIterations = 0.0f;
	static Int64 lastIterationTime = currentTimeMillis;

	numIterations += 1.0f;
	Int64 timeDelta = currentTimeMillis - lastIterationTime;

	if (bleLastIterationMillis.value() != timeDelta.low()) {
		bleLastIterationMillis.setValue(timeDelta.low());
	}
	unsigned newAvg = (unsigned) (((float) currentTimeMillis.low() / numIterations) + 0.5f);
	if (newAvg != bleAvgIterationMillis.value()) {
		bleAvgIterationMillis.setValue(newAvg);
	}

	if (abs(bleRpm.value() - rpm) > 10) {
		bleRpm.setValue(rpm);
	}

	lastIterationTime = currentTimeMillis;
#endif
}

void collect() {

	noInterrupts();
	currentTimeMillis = Timer::currentTimeMillis();
	interrupts();

#ifndef MOCK_OBD

	bool readOk = false;
	do {
		int numRead = obd.readPID(pids, PID_N, pidVal);
		readOk = numRead == PID_N;
	} while(!readOk);

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

#ifdef MOCK_APP
	DacConv::getInput();
	throttlePos = readInt();
#else
	readInt(); // ignore sent APP value
#endif

	engineTemp = readInt();
#endif

#ifndef MOCK_APP
	// read throttle pos from I2C ADC
	throttlePos = DacConv::getInput();
#endif

#ifdef MOCK_OBD
	Serial.print("tp");
	Serial.println(throttlePos);
#endif

	bleCollect();
}

bool breakRevMatch() {
	return throttlePos > 130;
}

int forceRpm() {
#ifdef ENABLE_BLE
	return bleForceRpm.value() * 100;
#else
	return 0;
#endif
}

}

