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
	bool m_running = false;
public:
	Leddar();
	virtual ~Leddar();
	void GetDetections();
	void StartAutomaticDetections();
	void StopAutomaticDetections();
};

#endif /* SRC_LEDDAR_H_ */
