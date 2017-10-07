///*
// * Leddar.h
// *
// *  Created on: Feb 3, 2016
// *      Author: Win7
// */
//
//#ifndef SRC_LEDDAR_H_
//#define SRC_LEDDAR_H_
//#include <iostream>
//#include <WPILib.h>
//#include <string>
//#include <array>
//using namespace std;
//
//struct point {
//	float x;
//	float y;
//};
//
//struct LineSeg {
//	point p1, p2, mid;
//	float angle;
//	float slope;
//	float yIntercept;
//	float length;
//	float distToMidpoint;
//};
//
//struct Detection {
//	float distance;
//	unsigned short amplitude;
//	unsigned char  detectionNumber;
//	unsigned char  flags;
//};
//
//class Leddar {
//private:
//	SerialPort m_RS232;
////	Task m_task;
//	// true if the detections array is not being populated.
//	bool m_safeToGet = false;
//	static vector<Detection> m_detections;
//
//	bool m_autoDetect = false;
//public:
//	Leddar();
//	virtual ~Leddar();
//
//	// Returns detections stored in buffer.
//	vector<point> GetDetections();
//	// fill the buffer with detections, get them from the buffer with GetDetections().
//	void FillBuffer();
//	void RequestDetections(); // send modbus request
//
//	unsigned GetLineSegs(LineSeg lineSeg[], point points[], const unsigned numPoints);
//
//	unsigned GetDetectionsAsCarteasion(point buff[], unsigned buffSize);
//	unsigned GetDetectionsAsLineSegs();
//	void StartAutoDetections(bool start);
//	bool IsAutoDetectEnabled();
//};
//
//#endif /* SRC_LEDDAR_H_ */
