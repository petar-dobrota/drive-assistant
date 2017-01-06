/*
 * DacConv.h
 *
 *  Created on: Jan 5, 2017
 *      Author: petard
 */

#ifndef DRIVE_ASSISTANT_DACCONV_H_
#define DRIVE_ASSISTANT_DACCONV_H_

#include "Wire.h"

#define DAC_ADDRESS (0x90 >> 1)
#define DAC_CMD 0x40

namespace DacConv {

void setOutput(unsigned outp);
unsigned getInput();

} /* namespace DacConv */

#endif /* DRIVE_ASSISTANT_DACCONV_H_ */
