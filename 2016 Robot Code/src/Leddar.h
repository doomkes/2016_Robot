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

struct point {
	float x;
	float y;
};
struct LineSeg {
	point p1, p2;
	float angle;
	float slope;
	float yIntercept;
};

class Leddar {
private:
	SerialPort m_RS_232;
public:
	Leddar();
	virtual ~Leddar();
	vector<Point> GetDetections();
	unsigned GetLineSegs(LineSeg lineSeg[], point points[], const unsigned numPoints);
};

#endif /* SRC_LEDDAR_H_ */
