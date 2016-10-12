#include "Int64.h"

Int64::Int64(long high, unsigned long low) {
	this->lowBytes = low;
	this->highBytes = high;
}

Int64::Int64(long num) {
	if (num < 0) {
		this->lowBytes = num;
		this->highBytes = -1;
	}
	else {
		this->lowBytes = num;
		this->highBytes = 0;
	}
}

Int64::Int64(int num) : Int64((long)num){
}

Int64::Int64() : lowBytes(0), highBytes(0) {
}

Int64 Int64::operator+(const Int64 &other) const {
	Int64 retVal;

	retVal.lowBytes = this->lowBytes + other.lowBytes;

	// check for overflow
	long overflow;
	if (retVal.lowBytes < this->lowBytes || retVal.lowBytes < other.lowBytes) {
		overflow = 1;
	}
	else {
		overflow = 0;
	}

	retVal.highBytes = this->highBytes + other.highBytes + overflow;

	return retVal;
}

Int64 Int64::operator-() const {
	Int64 retVal = *this;

	// decrement for one
	if (retVal.lowBytes == 0) {
		retVal.highBytes--;
	}
	retVal.lowBytes--;

	// invert bits
	retVal.lowBytes = ~retVal.lowBytes;
	retVal.highBytes = ~retVal.highBytes;

	return retVal;
}

Int64 Int64::operator-(const Int64 &x) const {
	return *this + (-x);
}

bool Int64::operator==(const Int64 &other) const {
	return (this->lowBytes == other.lowBytes) && (this->highBytes == other.highBytes);
}

bool Int64::operator!=(const Int64 &other) const {
	return !(*this == other);
}

bool Int64::operator<(const Int64 &other) const {
	if (this->highBytes < other.highBytes) {
		return true;
	} else if (this->highBytes == other.highBytes) {
		return this->lowBytes < other.lowBytes;
	} else {
		return false;
	}
}

bool Int64::operator>(const Int64 &other) const {
	if (this->highBytes > other.highBytes) {
		return true;
	} else if (this->highBytes == other.highBytes) {
		return this->lowBytes > other.lowBytes;
	} else {
		return false;
	}
}

bool Int64::operator<=(const Int64 &other) const {
	return (*this < other) || (*this == other);
}

bool Int64::operator>=(const Int64 &other) const {
	return (*this > other) || (*this == other);
}
