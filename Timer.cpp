#include <Arduino.h>
#include "Timer.h"

long Timer::numOverflows = 0;
unsigned long Timer::lastReadMillis = 0;
	
Int64 Timer::currentTimeMillis() {
	unsigned long currentMillis = millis();
	
	if (currentMillis < Timer::lastReadMillis) {
		Timer::numOverflows++;
	}
	
	Timer::lastReadMillis = currentMillis;
	
	return Int64(numOverflows, currentMillis);
}
