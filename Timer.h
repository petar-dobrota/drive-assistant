#pragma once

#include "Int64.h"

class Timer {
private:

	static long numOverflows;
	static unsigned long lastReadMillis;

public:

	static Int64 currentTimeMillis();

};
