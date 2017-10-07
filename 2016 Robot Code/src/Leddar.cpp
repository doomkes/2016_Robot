///*
// * Leddar.cpp
// *
// *  Created on: Feb 3, 2016
// *      Author: Win7
// */
//#define _USE_MATH_DEFINES
//#include <Leddar.h>
//#include <cmath>
//#define DEBUG_LEVEL 0xf
//
//vector<Detection> Leddar::m_detections;
//static void AutoDetect(Leddar *leddar) {
//	printf("Starting thread\n");
//	//cout << "Starting thread, cout" << endl;
//	leddar->RequestDetections(); // first time - send request
//	Wait(0.05);
//	unsigned count = 0;
//
//	while(true) {
//
//		//if(leddar->IsAutoDetectEnabled()) {
//			leddar->FillBuffer();
//			leddar->GetDetectionsAsLineSegs();
//			Wait(0.05);
//			leddar->RequestDetections();
//			printf("Leddar Loop %i\n",count++);
//		//}
//
//		Wait(10);
//	}
//}
//
//Leddar::Leddar() :
//	m_RS232(115200),
//	m_task("Leddar Thread", (FUNCPTR)AutoDetect)
//{
//
//}
//bool Leddar::IsAutoDetectEnabled() {
//	return m_autoDetect;
//}
//
//Leddar::~Leddar() {
//
//}
//
//
//void Leddar::StartAutoDetections(bool start) {
//	if(start) {
//		m_autoDetect = true;
//		printf("Leddar auto detect on.\n");
//	} else {
//		m_autoDetect = false;
//		printf("Leddar auto detect off.\n");
//	}
//}
//
//vector<point> Leddar::GetDetections() {
//	vector<point> cartesianDetections;
//	//Wait for the array to be safe to access.
////	while(!m_safeToGet)
////	{
////		Wait(0.05);
////	}
//	cout << "size: " << m_detections.size() << endl;
//	float angle, x, y;
//	for(unsigned i = 0; i < m_detections.size(); i++) {
//		angle = 1.5 + m_detections[i].detectionNumber*3 - 24;
//		x = sin(angle*M_PI/180)*m_detections[i].distance;
//		y = cos(angle*M_PI/180)*m_detections[i].distance;
//		cartesianDetections.push_back({x, y});
//	}
//	return cartesianDetections;
//}
//
//void Leddar::RequestDetections(){
//	m_RS232.Reset();
//	m_RS232.Flush();
//	//sending the bytes to the leddar sensor
//	char query[] = {0x01, 0x41, 0xC0, 0x10};
//	m_RS232.Write(query, 4);
//	if (DEBUG_LEVEL & 0x4)printf("Request Detections \n");
//}
//
//
//void Leddar::FillBuffer() {
//	unsigned bytesRecived, numDetections;
//	static char response[200];
//
//
//	bytesRecived = m_RS232.GetBytesReceived();
//	//SmartDashboard::PutNumber("bytes recived", bytesRecived);
//	if (DEBUG_LEVEL & 0x4) printf("Bytes recived: %i\n", bytesRecived);
//	//saving the data recived back from the leddar
//	//char response[bytesRecived];
//	if(bytesRecived >200)
//		bytesRecived = 200;
//	m_RS232.Read(response, bytesRecived);
//
//	if(response[0] != 0x01 || response[1] != 0x41) {
//		return;
//	}
//
////	SmartDashboard::PutNumber("byte 0 address", response[0]);
////	SmartDashboard::PutNumber("byte 1 function code", response[1]);
////	SmartDashboard::PutNumber("byte 2 # of detections", response[2]);
//	if (DEBUG_LEVEL & 0x4) printf("address: %i function %i Det: %i \n",response[0],response[1],response[2]);
//
//	m_safeToGet = false;
//	numDetections = response[2];
//	char *rawDetections = response + 3;
//	Detection detection;
//	m_detections.clear();
//	for(unsigned i = 0; i < numDetections; i++) {
//		detection.distance			= (rawDetections[i*5] | (rawDetections[i*5+1] << 8)) / 2.54;
//		detection.amplitude			= (rawDetections[i*5+2] | (rawDetections[i*5+1+2] << 8))/64;
//		detection.detectionNumber	= (rawDetections[i*5+4] & 0xF0)>>4;
//		detection.flags				= rawDetections[i*5+4] & 0x0F;
//		m_detections.push_back(detection);
//
//	}
//	if (DEBUG_LEVEL & 0x2){
//		if(m_detections.size() >=16 ){
//		printf("dist %f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t \n",
//				m_detections[0].distance,
//				m_detections[1].distance,
//				m_detections[2].distance,
//				m_detections[3].distance,
//				m_detections[4].distance,
//				m_detections[5].distance,
//				m_detections[6].distance,
//				m_detections[7].distance,
//				m_detections[8].distance,
//				m_detections[9].distance,
//				m_detections[10].distance,
//				m_detections[11].distance,
//				m_detections[12].distance,
//				m_detections[13].distance,
//				m_detections[14].distance,
//				m_detections[15].distance);
//		printf("det # %i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t \n",
//				m_detections[0].detectionNumber,
//				m_detections[1].detectionNumber,
//				m_detections[2].detectionNumber,
//				m_detections[3].detectionNumber,
//				m_detections[4].detectionNumber,
//				m_detections[5].detectionNumber,
//				m_detections[6].detectionNumber,
//				m_detections[7].detectionNumber,
//				m_detections[8].detectionNumber,
//				m_detections[9].detectionNumber,
//				m_detections[10].detectionNumber,
//				m_detections[11].detectionNumber,
//				m_detections[12].detectionNumber,
//				m_detections[13].detectionNumber,
//				m_detections[14].detectionNumber,
//				m_detections[15].detectionNumber);
//		printf("amp # %i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t \n",
//				m_detections[0].amplitude,
//				m_detections[1].amplitude,
//				m_detections[2].amplitude,
//				m_detections[3].amplitude,
//				m_detections[4].amplitude,
//				m_detections[5].amplitude,
//				m_detections[6].amplitude,
//				m_detections[7].amplitude,
//				m_detections[8].amplitude,
//				m_detections[9].amplitude,
//				m_detections[10].amplitude,
//				m_detections[11].amplitude,
//				m_detections[12].amplitude,
//				m_detections[13].amplitude,
//				m_detections[14].amplitude,
//				m_detections[15].amplitude);
//		printf("flag # %i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t \n",
//				m_detections[0].flags,
//				m_detections[1].flags,
//				m_detections[2].flags,
//				m_detections[3].flags,
//				m_detections[4].flags,
//				m_detections[5].flags,
//				m_detections[6].flags,
//				m_detections[7].flags,
//				m_detections[8].flags,
//				m_detections[9].flags,
//				m_detections[10].flags,
//				m_detections[11].flags,
//				m_detections[12].flags,
//				m_detections[13].flags,
//				m_detections[14].flags,
//				m_detections[15].flags);
//		}
//	}
////	if(m_detections.size() >=8 ) {
//		//SmartDashboard::PutNumber("distance of detection 8", m_detections[8].distance);
////		printf("dist det 8 %i\n",m_detections[8].distance);
////	}
//	m_safeToGet = true;
//}
//
//unsigned Leddar::GetLineSegs(LineSeg lineSeg[], point points[], const unsigned numPoints) {
//	unsigned startIndex = 99;
//	vector<unsigned> startIndexes, endIndexes;
//
//	unsigned foundFlags[numPoints] = {0};
//	unsigned foundFlag = 1;
//	bool lineFound = false;
//
//	for(unsigned i = 0; i < numPoints-2 && numPoints>=2; i++) {
//		float error = fabs((points[i+1].y - points[i].y)*(points[i+2].x - points[i+1].x) - (points[i+2].y-points[i+1].y)*(points[i+1].x-points[i].x));
//		cout << "Error: " << error << endl;
//		if(error < 0.0075) {
//			if (lineFound == false) {
//				startIndex = i;
//			}
//			foundFlags[i]   |= foundFlag;
//			foundFlags[i+1] |= foundFlag;
//			foundFlags[i+2] |= foundFlag;
//			cout << "adding point " << i+2 << " to line" << endl;
//			lineFound = true;
//			if(i == 13) {
//				startIndexes.push_back(startIndex);
//				endIndexes.push_back(i+2);
//			}
//		}
//		else if(lineFound) {
//			cout << "line found" << endl;
//			foundFlag <<= 1;
//			lineFound = false;
//			startIndexes.push_back(startIndex);
//			endIndexes.push_back(i+2);
//		}
//	}
//
//	for(unsigned i = 0; i < numPoints; i++) {
//		cout << "flag " << i << " "<< foundFlags[i] << endl;
//	}
//
//	if(endIndexes.size() < startIndexes.size()) {
//		endIndexes.push_back(15);
//	}
//
//	for(unsigned i = 0; i < startIndexes.size() && i < endIndexes.size(); i++) {
//		lineSeg[i].p1 = points[startIndexes[i]];
//		lineSeg[i].p2 = points[endIndexes[i]];
//
//		float run = lineSeg[i].p2.x - lineSeg[i].p1.x;
//		if(run != 0) {
//			lineSeg[i].slope = (lineSeg[i].p2.y - lineSeg[i].p1.y) / run;
//		}
//		else {
//			lineSeg[i].slope = 99999999; //for out purposes, this will work fine.
//		}
//		lineSeg[i].yIntercept = (lineSeg[i].p1.y-lineSeg[i].slope * lineSeg[i].p1.x);
//		lineSeg[i].angle = atan2(lineSeg[i].p2.y - lineSeg[i].p1.y, run);
//		lineSeg[i].length = sqrt(pow(lineSeg[i].p1.x - lineSeg[i].p2.x, 2)
//								 + pow(lineSeg[i].p1.y - lineSeg[i].p2.y, 2));
//
//	}
//	return min(startIndexes.size(), endIndexes.size());
//}
//unsigned Leddar::GetDetectionsAsCarteasion(point buff[], unsigned buffSize) {
//	//return 0;
//	//point detections[m_detections.size()];
//	float angle, x, y;
//	printf("xy data:\n");
//	for(unsigned i = 0; i < buffSize; i++) {
//		angle = 1.5 + m_detections[i].detectionNumber*3 - 24;
//		x = sin(angle*M_PI/180)*m_detections[i].distance; // Leddar offset 7 inches in x and 3 in y
//		y = cos(angle*M_PI/180)*m_detections[i].distance;
//		buff[i] = {x, y};
//		printf("%i,%f,%f \n",i,x,y);
//	}
//	printf("end xy data\n");
//	return m_detections.size();
//}
//unsigned Leddar::GetDetectionsAsLineSegs() {
//
//	unsigned startIndex = 99;
//	LineSeg lineSeg[16]; // really can't have more than 14 with 16 points
//	vector<unsigned> startIndexes, endIndexes;
//	unsigned numPoints = m_detections.size();
//	point points[numPoints];
//	GetDetectionsAsCarteasion(points, numPoints);
//	unsigned foundFlags[numPoints] = {0};
//	unsigned foundFlag = 1;
//	bool lineFound = false;
//
//	for(unsigned i = 0; i < numPoints-2 && numPoints>=2; i++) {
//		float error = fabs((points[i+1].y - points[i].y)*(points[i+2].x - points[i+1].x) - (points[i+2].y-points[i+1].y)*(points[i+1].x-points[i].x));
//		if (DEBUG_LEVEL & 0x4)cout << "Error: " << error << endl;
//		if(error < 15) {
//			if (lineFound == false) {
//				startIndex = i;
//			}
//			foundFlags[i]   |= foundFlag;
//			foundFlags[i+1] |= foundFlag;
//			foundFlags[i+2] |= foundFlag;
//			if (DEBUG_LEVEL & 0x4)cout << "adding point " << i+2 << " to line" << endl;
//			lineFound = true;
//			if(i == 13) {
//				startIndexes.push_back(startIndex);
//				endIndexes.push_back(i+2);
//			}
//		}
//		else if(lineFound) {
//			if (DEBUG_LEVEL & 0x4)cout << "line found" << endl;
//			foundFlag <<= 1;
//			lineFound = false;
//			startIndexes.push_back(startIndex);
//			endIndexes.push_back(i+1);
//		}
//	}
//
//	if(endIndexes.size() < startIndexes.size()) {
//		endIndexes.push_back(15);
//	}
//
//	unsigned num_segs = startIndexes.size();
//	for(unsigned i = 0; i < startIndexes.size() && i < endIndexes.size(); i++) {
//		lineSeg[i].p1 = points[startIndexes[i]];
//		lineSeg[i].p2 = points[endIndexes[i]];
//		if (DEBUG_LEVEL & 0x1)printf ("Line start %f,%f Line end %f,%f\n",lineSeg[i].p1.x,lineSeg[i].p1.y,lineSeg[i].p2.x,lineSeg[i].p2.y);
//
//		float run = lineSeg[i].p2.x - lineSeg[i].p1.x;
//		if(run != 0) {
//			lineSeg[i].slope = (lineSeg[i].p2.y - lineSeg[i].p1.y) / run;
//		}
//		else {
//			lineSeg[i].slope = 99999999; //for out purposes, this will work fine.
//		}
//		lineSeg[i].yIntercept = (lineSeg[i].p1.y-lineSeg[i].slope * lineSeg[i].p1.x);
//		lineSeg[i].angle = atan2(lineSeg[i].p2.y - lineSeg[i].p1.y, run);
//		lineSeg[i].length = sqrt(pow(lineSeg[i].p1.x - lineSeg[i].p2.x, 2)
//								 + pow(lineSeg[i].p1.y - lineSeg[i].p2.y, 2));
//
//		lineSeg[i].mid.x = (lineSeg[i].p1.x + lineSeg[i].p2.x)/2.0;
//		lineSeg[i].mid.y =  (lineSeg[i].p1.y + lineSeg[i].p2.y)/2.0;
//
//		lineSeg[i].distToMidpoint = sqrt(lineSeg[i].mid.x*lineSeg[i].mid.x + lineSeg[i].mid.y*lineSeg[i].mid.y);
//		if (DEBUG_LEVEL & 0x1)printf("line %i yint %f angle %f length %f dist %f\n\n",i,lineSeg[i].yIntercept,lineSeg[i].angle*180/3.14159,lineSeg[i].length,lineSeg[i].distToMidpoint);
//
//	}
//	float shortest = 999;
//	unsigned closestIndex = 0;
//	for(unsigned i = 0; i < num_segs; i++) {
//		if ((lineSeg[i].distToMidpoint < shortest) && (fabs(lineSeg[i].angle*180/3.14159) < 15.0)){
//			shortest = lineSeg[i].distToMidpoint;
//			closestIndex = i;
//			if (DEBUG_LEVEL & 0x4)printf("checking\n");
//		}
//	}
//	printf("Tower line %i yint %f angle %f length %f\n\n",closestIndex,lineSeg[closestIndex].yIntercept,lineSeg[closestIndex].angle*180/3.14159,lineSeg[closestIndex].length);
//	printf("Distance to tower %f",lineSeg[closestIndex].distToMidpoint);
//
//	float angleError;
//	angleError = atan2(lineSeg[closestIndex].distToMidpoint,lineSeg[closestIndex].mid.x)*180/3.14159 - 90;
//	printf ("Angle Error %f \n", angleError);
//
//
////	for(unsigned i = 0; i < m_detections.size(); i++) {
//	//	cout << m_detections[i].detectionNumber
//		//	 << ", " << m_detections[i].amplitude
////			 << ", " << m_detections[i].distance
////			 << ", " << m_detections[i].flags
////			 << endl;
////	}
//	return min(startIndexes.size(), endIndexes.size());
//}
