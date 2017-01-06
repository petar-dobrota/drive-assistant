/*
 * DacConv.cpp
 *
 *  Created on: Jan 5, 2017
 *      Author: petard
 */

#include "DacConv.h"

namespace DacConv {

unsigned output = 0;

void setOutput(unsigned outp) {
	output = outp;
	Wire.beginTransmission(DAC_ADDRESS);
	Wire.write(DAC_CMD);
	Wire.write(outp);
	Wire.endTransmission();
}

unsigned getInput() {
	Wire.beginTransmission(DAC_ADDRESS);
	Wire.write(DAC_CMD);
	Wire.write(output); // just write last output again
	Wire.endTransmission();
	Wire.requestFrom(DAC_ADDRESS, 2);
	return Wire.read();
}

} /* namespace DacConv */
