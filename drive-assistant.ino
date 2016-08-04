#include <Arduino.h>
#include <Wire.h>
#include <OBD.h>

// obd adapter pinout:
// rx - white
// tx - green

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTANTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// PINS
const unsigned ALARM_PIN = 13;
const unsigned ALARM_SPEAKER_PIN = 10;
const unsigned CLUTCH_DOWN_PIN = 9;
const unsigned CLUTCH_TOUCH_PIN = 8;

// TEST: DELETE
const unsigned TEST_0_PIN = 2;
const unsigned TEST_1_PIN = 3;
const unsigned TEST_2_PIN = 4;
const unsigned TEST_IN = 5;

// SETTINGS
const int REV_LIMIT = 2500; // TODO: change to 6000
const unsigned ALARM_T_MICROS = 1000000 / 1500;
const unsigned NUM_GEARS = 5;
const float GEAR_RATIOS[NUM_GEARS] = {98.4252f, 59.5613f, 40.1856f, 30.6805f, 23.5658f}; // gearRatio=rpm/v
const float GEAR_RATIO_TOLERANCE = 0.15f; // gear ratio tolerance when determinating in which gear car is 

// rpmToThrottle function
const int rpmToThrottleDomain = 7000;
const int rpmToThrottleN = 120;
// TODO: add actual data
const float rpmToThrottleFunction[rpmToThrottleN] = {1.2f};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

COBD obd;

float currRpm;
int currSpeed;
float dt; // delta time between iterations in micro seconds

void setup()
{

  pinMode(ALARM_PIN, OUTPUT);
  pinMode(ALARM_SPEAKER_PIN, OUTPUT);

  pinMode(CLUTCH_DOWN_PIN, INPUT);
  pinMode(CLUTCH_TOUCH_PIN, INPUT);
  
  pinMode(TEST_0_PIN, OUTPUT);
  pinMode(TEST_1_PIN, OUTPUT);
  pinMode(TEST_2_PIN, OUTPUT);
  pinMode(TEST_IN, INPUT);
  
  digitalWrite(TEST_0_PIN, HIGH);
  digitalWrite(TEST_1_PIN, HIGH);
  digitalWrite(TEST_2_PIN, HIGH);

  obd.begin();
  while (!obd.init());  

  digitalWrite(TEST_1_PIN, LOW);
}

// Determinates how much preasure throttle pedal needs to reach specified RPM when engine isn't loaded.
float rpmToThrottle(int rpm) {
  // rpm on X, throttle on Y
  
  float xStep = (float) rpmToThrottleDomain / rpmToThrottleN;

  float mx = rpm / xStep;
  int lx = (int) mx;
  int hx = lx + 1;
  
  if (hx < rpmToThrottleN) {
    float ly = rpmToThrottleFunction[lx];
    float hy = rpmToThrottleFunction[hx];

    return ((mx - lx) * (hy - ly) / (hx - lx)) + ly;
  } else {
    // desired rpm is way to high, probably error, return 0 to not overrev the engine
    return 0;
  }
}

void showGear(int g) {

  if (g == 1) {
    digitalWrite(TEST_0_PIN, HIGH);
    digitalWrite(TEST_1_PIN, LOW);
    digitalWrite(TEST_2_PIN, LOW);
  } else if (g==2){
    digitalWrite(TEST_0_PIN, LOW);
    digitalWrite(TEST_1_PIN, HIGH);
    digitalWrite(TEST_2_PIN, LOW); 
  } else if (g==3){
    digitalWrite(TEST_0_PIN, LOW);
    digitalWrite(TEST_1_PIN, LOW);
    digitalWrite(TEST_2_PIN, HIGH);
  } else if (g==-1) {
    // no leds
    digitalWrite(TEST_0_PIN, LOW);
    digitalWrite(TEST_1_PIN, LOW);
    digitalWrite(TEST_2_PIN, LOW);
  }
}

void setThrottlePos(float newPos) {
  // TODO: Also check and cap range of newPos
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// General functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int obdRead(byte pid) {
  int value;
  if (obd.read(pid, value)) {
    return value;
  } else {
    // TODO: Stop exec
    return -1;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sub coroutines: Called from main coroutines. Routines are doing it's thing while being called.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void alarmRinging() {
  static unsigned long lastTick = 0;

  unsigned long now = micros();

  if (lastTick > now) {
    // timer overflow, hack for this periode
    lastTick = now;
  }

  if (lastTick + ALARM_T_MICROS <= now) {
    // whole periode passed
    digitalWrite(ALARM_SPEAKER_PIN, !digitalRead(ALARM_SPEAKER_PIN));
    lastTick = now;
  }
}

void rpmSetting(float desiredRpm) {
  float diff = (float) (currRpm - desiredRpm) / (currRpm + desiredRpm);
  if (diff < 0) {
    diff = -diff;
  }

  float out;
  if (diff < 0.15f) {
    out = rpmToThrottle(desiredRpm);
  } else if(currRpm < desiredRpm) {
    out = 1;
  } else {
    out = 0;
  }

  setThrottlePos(out);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN CORUTINES: 
// Will be called from main loop.
// Return true if no other co-routines should be called per a this loop
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int lastGear;
bool gearMonitoring() {

  float currRatio = currRpm / currSpeed;
  int gear = -1;
  for (int i = 0; i < NUM_GEARS; i++) {
    float maxDelta = GEAR_RATIOS[i] * GEAR_RATIO_TOLERANCE;
    if ((currRatio > GEAR_RATIOS[i] - maxDelta) && (currRatio < GEAR_RATIOS[i] + maxDelta)) {
      gear = i + 1;
      break;
    }
  }
  if (gear != -1) {
    if (gear != lastGear) {
      showGear(gear);
      lastGear = gear;
    }
  }
  
  return false;
}

bool highRevNotifying() {

  if (currRpm > REV_LIMIT) {
     digitalWrite(ALARM_PIN, HIGH);
     alarmRinging();
  } else {
    // alarm off
    digitalWrite(ALARM_PIN, LOW);
    digitalWrite(ALARM_SPEAKER_PIN, LOW);
  }

  // high revs notify doesn't interfere with other co-routines
  return false;
}

bool shouldRevMatch() {
  // rev match should be done if:
  // - clutch is pressed
  // - !revmatching is currently executing more than 2s
  // - engine temp reached 75C

  // clutch is pressed
  if (!digitalRead(CLUTCH_DOWN_PIN)) {
    return false;
  }

  
  
  return true;
}

bool revMatching() {
  if(!shouldRevMatch) {
    return false;
  }
  
  // TODO:
  
  return false;
}

bool cruiseControling() {
  // TODO:
  
  return false;
}

bool launchControling() {
  // TODO:
  
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOOP
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{

  // TODO: OBD READS
  currRpm = obdRead(PID_RPM);
  currSpeed = obdRead(PID_SPEED);
  
  if (gearMonitoring()) {
    return;
  }
  
  if (highRevNotifying()) {
    return;
  }

  if (revMatching()) {
    return;
  }

  if (cruiseControling()) {
    return;
  }

  if (launchControling()) {
    return;
  }
  
}
