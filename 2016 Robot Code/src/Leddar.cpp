/*
 * Leddar.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: Win7
 */
#define _USE_MATH_DEFINES
#include <Leddar.h>
#include <cmath>
Leddar::Leddar() :
	m_RS232(115200),
	m_task("Leddar Thread", (FUNCPTR)Leddar::AutoDetect)
{

}


Leddar::~Leddar() {

}

void Leddar::AutoDetect(Leddar *leddar) {
	while(leddar->m_autoDetect) {
		leddar->FillBuffer();
	}
}
void Leddar::StartAutoDetections(bool start) {
	if(start) {
		m_autoDetect = true;
	} else {
		m_autoDetect = false;
	}
}

vector<point> Leddar::GetDetections() {
	vector<point> cartesianDetections;
	//Wait for the array to be safe to access.
	while(!m_safeToGet)
	{ }

	float angle, x, y;
	for(unsigned i = 0; i < m_detections.size(); i++) {
		angle = 1.5 + m_detections[i].detectionNumber*3 - 24;
		x = sin(angle*M_PI/180)*m_detections[i].distance;
		y = cos(angle*M_PI/180)*m_detections[i].distance;
		cartesianDetections.push_back({x, y});
	}
	return cartesianDetections;
}

void Leddar::FillBuffer() {
	unsigned bytesRecived, numDetections;

	m_RS232.Reset();
	//sending the bytes to the leddar sensor
	char query[] = {0x01, 0x41, 0xC0, 0x10};
	m_RS232.Write(query, 4);

	Wait(0.250);
	//This is causing too much of a delay on the drive

	bytesRecived = m_RS232.GetBytesReceived();
	//SmartDashboard::PutNumber("bytes recived", bytesRecived);

	//saving the data recived back from the leddar
	char response[bytesRecived];
	m_RS232.Read(response, bytesRecived);

	if(response[0] != 0x01 || response[1] != 0x41) {
		return;
	}

//	SmartDashboard::PutNumber("byte 0 address", response[0]);
//	SmartDashboard::PutNumber("byte 1 function code", response[1]);
//	SmartDashboard::PutNumber("byte 2 # of detections", response[2]);

	m_safeToGet = false;

	numDetections = response[2];
	char *rawDetections = response + 3;
	Detection detection;

	m_detections.resize(numDetections);
	for(unsigned i = 0; i < numDetections; i++) {
		detection.distance			= rawDetections[i*5] | (rawDetections[i*5+1] << 8);
		detection.amplitude			= (rawDetections[i*5+2] | (rawDetections[i*5+1+2] << 8))/64;
		detection.detectionNumber	= rawDetections[i*5+4] & 0xF0;
		detection.flags				= rawDetections[i*5+4] & 0x0F;
		m_detections[i] = detection;
	}
	m_safeToGet = true;
}

unsigned Leddar::GetLineSegs(LineSeg lineSeg[], point points[], const unsigned numPoints) {
	unsigned startIndex = 99;
	vector<unsigned> startIndexes, endIndexes;

	unsigned foundFlags[numPoints] = {0};
	unsigned foundFlag = 1;
	bool lineFound = false;

	for(unsigned i = 0; i < numPoints-2 && numPoints>=2; i++) {
		float error = fabs((points[i+1].y - points[i].y)*(points[i+2].x - points[i+1].x) - (points[i+2].y-points[i+1].y)*(points[i+1].x-points[i].x));
		cout << "Error: " << error << endl;
		if(error < 0.0075) {
			if (lineFound == false) {
				startIndex = i;
			}
			foundFlags[i]   |= foundFlag;
			foundFlags[i+1] |= foundFlag;
			foundFlags[i+2] |= foundFlag;
			cout << "adding point " << i+2 << " to line" << endl;
			lineFound = true;
			if(i == 13) {
				startIndexes.push_back(startIndex);
				endIndexes.push_back(i+2);
			}
		}
		else if(lineFound) {
			cout << "line found" << endl;
			foundFlag <<= 1;
			lineFound = false;
			startIndexes.push_back(startIndex);
			endIndexes.push_back(i+2);
		}
	}

	for(unsigned i = 0; i < numPoints; i++) {
		cout << "flag " << i << " "<< foundFlags[i] << endl;
	}

	if(endIndexes.size() < startIndexes.size()) {
		endIndexes.push_back(15);
	}

	for(unsigned i = 0; i < startIndexes.size() && i < endIndexes.size(); i++) {
		lineSeg[i].p1 = points[startIndexes[i]];
		lineSeg[i].p2 = points[endIndexes[i]];

		float run = lineSeg[i].p2.x - lineSeg[i].p1.x;
		if(run != 0) {
			lineSeg[i].slope = (lineSeg[i].p2.y - lineSeg[i].p1.y) / run;
		}
		else {
			lineSeg[i].slope = 99999999; //for out purposes, this will work fine.
		}
		lineSeg[i].yIntercept = (lineSeg[i].p1.y-lineSeg[i].slope * lineSeg[i].p1.x);
		lineSeg[i].angle = atan2(lineSeg[i].p2.y - lineSeg[i].p1.y, run);

	}
	return min(startIndexes.size(), endIndexes.size());
}
