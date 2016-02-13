/*
 * Leddar.h
 *
 *  Created on: Feb 3, 2016
 *      Author: Win7
 */

#ifndef SRC_LEDDAR_H_
#define SRC_LEDDAR_H_
#include <iostream>
#include <wpilib.h>
#include <string>
#include <cstdint>
#include <vector>
using namespace std;

struct Detection {
	uint16_t distance;
	uint16_t amplitude;
	uint8_t flags;
};

class Leddar {
private:
	SerialPort m_RS_232;
public:
	Leddar();
	virtual ~Leddar();
	vector<Detection> GetDetections();
};

#endif /* SRC_LEDDAR_H_ */
