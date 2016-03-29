/*
 * Leddar.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: Win7
 */
#define _USE_MATH_DEFINES
#include <Leddar.h>
#include <cmath>
vector<Detection> Leddar::m_detections;
static void AutoDetect(Leddar *leddar) {
	printf("Starting thread\n");
	//cout << "Starting thread, cout" << endl;
	while(true) {

		//while(leddar->IsAutoDetectEnabled()) {
			leddar->FillBuffer();
			//cout << "Leddar Count(cout) " << leddarCount << endl;
		//}

		Wait(0.0005);
	}
}

Leddar::Leddar() :
	m_RS232(115200),
	m_task("Leddar Thread", (FUNCPTR)AutoDetect)
{

}
bool Leddar::IsAutoDetectEnabled() {
	return m_autoDetect;
}

Leddar::~Leddar() {

}


void Leddar::StartAutoDetections(bool start) {
	if(start) {
		m_autoDetect = true;
		printf("Leddar auto detect on.");
	} else {
		m_autoDetect = false;
		printf("Leddar auto detect off.");
	}
}

vector<point> Leddar::GetDetections() {
	vector<point> cartesianDetections;
	//Wait for the array to be safe to access.
//	while(!m_safeToGet)
//	{
//		Wait(0.05);
//	}
	cout << "size: " << m_detections.size() << endl;
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

	//Wait(0.250);
	while(!m_RS232.GetBytesReceived()) //TODO: add timeout.
	{}
	Wait(0.010);
	bytesRecived = m_RS232.GetBytesReceived();
	//SmartDashboard::PutNumber("bytes recived", bytesRecived);
	//printf("Bytes recived: %i\n", bytesRecived);
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
	m_detections.clear();
	for(unsigned i = 0; i < numDetections; i++) {
		detection.distance			= rawDetections[i*5] | (rawDetections[i*5+1] << 8);
		detection.amplitude			= (rawDetections[i*5+2] | (rawDetections[i*5+1+2] << 8))/64;
		detection.detectionNumber	= rawDetections[i*5+4] & 0xF0;
		detection.flags				= rawDetections[i*5+4] & 0x0F;
		m_detections.push_back(detection);

	}
	if(m_detections.size() >=8 ) {
		//SmartDashboard::PutNumber("distance of detection 8", m_detections[8].distance);
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
		lineSeg[i].length = sqrt(pow(lineSeg[i].p1.x - lineSeg[i].p2.x, 2)
								 + pow(lineSeg[i].p1.y - lineSeg[i].p2.y, 2));

	}
	return min(startIndexes.size(), endIndexes.size());
}
unsigned Leddar::GetDetectionsAsCarteasion(point buff[], unsigned buffSize) {
	//return 0;
	//point detections[m_detections.size()];
	float angle, x, y;
	for(unsigned i = 0; i < buffSize; i++) {
		angle = 1.5 + m_detections[i].detectionNumber*3 - 24;
		x = sin(angle*M_PI/180)*m_detections[i].distance;
		y = cos(angle*M_PI/180)*m_detections[i].distance;
		buff[i] = {x, y};
	}
	return m_detections.size();
}
unsigned Leddar::GetDetectionsAsLineSegs(LineSeg lineSeg[], unsigned buffSize) {

	unsigned startIndex = 99;

	vector<unsigned> startIndexes, endIndexes;
	unsigned numPoints = m_detections.size();
	point points[numPoints];
	GetDetectionsAsCarteasion(points, numPoints);
	unsigned foundFlags[numPoints] = {0};
	unsigned foundFlag = 1;
	bool lineFound = false;

	for(unsigned i = 0; i < numPoints-2 && numPoints>=2; i++) {
		float error = fabs((points[i+1].y - points[i].y)*(points[i+2].x - points[i+1].x) - (points[i+2].y-points[i+1].y)*(points[i+1].x-points[i].x));
		cout << "Error: " << error << endl;
		if(error < 10000) {
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
		lineSeg[i].length = sqrt(pow(lineSeg[i].p1.x - lineSeg[i].p2.x, 2)
								 + pow(lineSeg[i].p1.y - lineSeg[i].p2.y, 2));

	}
	for(unsigned i = 0; i < m_detections.size(); i++) {
		cout << m_detections[i].detectionNumber
			 << ", " << m_detections[i].amplitude
			 << ", " << m_detections[i].distance
			 << ", " << m_detections[i].flags
			 << endl;
	}
	return min(startIndexes.size(), endIndexes.size());
}
