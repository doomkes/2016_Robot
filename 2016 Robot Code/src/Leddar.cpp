/*
 * Leddar.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: Win7
 */

#include <Leddar.h>



Leddar::Leddar() :
	m_RS_232(115200)
{

}


Leddar::~Leddar() {

}


vector<Detection> Leddar::GetDetections() {
	vector<Detection> detections;
	m_RS_232.Reset();
	char query[] = {0x01, 0x41,0xC0,0x10};
	m_RS_232.Write(query, 4);

	Wait(0.05);
	unsigned bytesRecived = m_RS_232.GetBytesReceived();
	SmartDashboard::PutNumber("bytes recived", bytesRecived);

	char response[bytesRecived];
	m_RS_232.Read(response, bytesRecived);
	SmartDashboard::PutNumber("byte 6", response[5]);
	SmartDashboard::PutNumber("byte 7", response[6]);
	SmartDashboard::PutNumber("byte 8", response[7]);
	SmartDashboard::PutNumber("byte 9", response[8]);
	SmartDashboard::PutNumber("byte A", response[9]);



//	cout << "bytes recived: " << bytesRecived << endl;
//	for(int i = 0; i < bytesRecived; i++) {
//		cout << hex << response[i] << " ";
//	}
//	cout << endl;

//	char numDetections;
//	m_RS_232.Read(&numDetections, 1);
//
//	cout << "Num Detections: " << numDetections << endl;
//	SmartDashboard::PutNumber("numDetections", numDetections);
//	char buff[numDetections] = {0};
//	m_RS_232.Read(buff, numDetections*5);
//
//	detections.resize(numDetections);
//	memcpy(detections.data(), buff, numDetections*5);
//
//	for(unsigned i = 0; i < detections.size(); i++) {
//		cout << "distance of detection " << i
//			<<": " << detections[i].distance << endl;
//	}

	return detections;
}
