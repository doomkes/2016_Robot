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
	char function[] = {0x01, 0x41, 0x00, 0x00, 0x51, 0xCC};
	m_RS_232.Reset();
	m_RS_232.Write(function, sizeof function);
	Wait(0.1);

	//char numDetections;
	//m_RS_232.Read(&numDetections, 1);

	SmartDashboard::PutNumber("bytes recived", m_RS_232.GetBytesReceived());
//	char buff[numDetections*5] = {0};
//	m_RS_232.Read(buff, numDetections*5);
//
//	vector<Detection> detections(numDetections*5);
//	memcpy(detections.data(), buff, numDetections*5);

	vector<Detection> asdf;
	return asdf;
}
