#include "RpmToThrottleFunction.h"

namespace RpmToThrottleFunction {

// Determinates how much pressure throttle pedal needs to reach specified RPM when engine isn't loaded.
int getThrottle(float rpm) {

	float app;

	if (rpm > 7200.0f) {
		app = 77.0f;
	} else if (rpm > 3783.0f) {
		app = (rpm + 14419.0f) / 281.0f;
	} else if (rpm > 1600.0f) {
		app = (rpm + 16143.34247f) / 311.1712329f;
	} else if (rpm > 1000.0f) {
		app = (rpm + 6668.75f) / 145.25f;
	} else if (rpm > 800.0f) {
		app = (rpm + 2692.214286f) / 69.64285714f;
	} else {
		app = 40;
	}

	return (int) (app + 0.5f);
}

}
