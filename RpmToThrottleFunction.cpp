#include "RpmToThrottleFunction.h"

const int rpmToThrottleDomain = 6000;
const int rpmToThrottleN = 120;
// TODO: add actual data
const int rpmToThrottleFunction[rpmToThrottleN] = {40};

// Determinates how much pressure throttle pedal needs to reach specified RPM when engine isn't loaded.
int RpmToThrottleFunction::getThrottle(float rpm) {
  // rpm on X, throttle on Y
  
  float xStep = (float) rpmToThrottleDomain / rpmToThrottleN;

  float mx = rpm / xStep;
  int lx = (int) mx;
  int hx = lx + 1;
  
  if (hx < rpmToThrottleN) {
    float ly = rpmToThrottleFunction[lx];
    float hy = rpmToThrottleFunction[hx];

    return (int) ((((mx - lx) * (hy - ly) / (hx - lx)) + ly) + 0.5f);
  } else {
    // desired rpm is way to high, probably error, return 0 to not overrev the engine
    return 0;
  }
}
