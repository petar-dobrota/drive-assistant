#include "RpmToThrottleFunction.h"

const int rpmToThrottleDomain = 6000;
const int rpmToThrottleN = 78;
// TODO: add actual data
const int rpmToThrottleFunction[rpmToThrottleN] = { 0, 2, 4, 6, 8, 10, 12, 14,
		16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50,
		52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86,
		88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116,
		118, 120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142, 144,
		146, 148, 150, 152, 154 };

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
