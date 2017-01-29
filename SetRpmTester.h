/*
 * SetRpmTester.h
 *
 *  Created on: Jan 25, 2017
 *      Author: petard
 */

#ifndef DRIVE_ASSISTANT_SETRPMTESTER_H_
#define DRIVE_ASSISTANT_SETRPMTESTER_H_

#include "EngineControl.h"

namespace SetRpmTester {

	bool begin();
	bool setRpmTesting(EngineControl *engine);

}

#endif /* DRIVE_ASSISTANT_SETRPMTESTER_H_ */
