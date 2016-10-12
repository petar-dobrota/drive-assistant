#pragma once

class Int64 {
private:

	long highBytes;
	unsigned long lowBytes;
	
public:

	Int64 operator+(const Int64 &other) const;
	Int64 operator-() const;
	Int64 operator-(const Int64 &x) const;
	
	bool operator==(const Int64 &other) const;
	bool operator!=(const Int64 &other) const;
	
	bool operator<(const Int64 &other) const;
	bool operator>(const Int64 &other) const;
	bool operator<=(const Int64 &other) const;
	bool operator>=(const Int64 &other) const;
	
	Int64(long high, unsigned long low);
	Int64(long from);
	Int64(int num);
	Int64();

};