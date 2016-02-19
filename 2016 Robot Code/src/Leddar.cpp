/*
 * Leddar.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: Win7
 */

#include <Leddar.h>
#include <sstream>

Leddar::Leddar() :
	m_RS_232(115200)
{

}


Leddar::~Leddar() {

}


void Leddar::GetDetections() {

	m_RS_232.Reset();
	//sending the bytes to the leddar sensor
	char query[] = {0x01, 0x41,0xC0,0x10};
	m_RS_232.Write(query, 4);


	Wait(0.5);

	unsigned bytesRecived = m_RS_232.GetBytesReceived();
	SmartDashboard::PutNumber("bytes recived", bytesRecived);

	//saving the data recived back from the leddar
	char response[bytesRecived];
	m_RS_232.Read(response, bytesRecived);

	unsigned numDetections = response[2];

	char Detections[numDetections*5];
	memcpy(Detections, response+3, numDetections*5);

	if(response[0] != 0x01 || response[1] != 0x41) {
		return;
	}

	//putting it on the SmartDashboard so that we could see a few of the bytes we were getting back
	SmartDashboard::PutNumber("byte 0 address", response[0]);
	SmartDashboard::PutNumber("byte 1 function code", response[1]);
	SmartDashboard::PutNumber("byte 2 # of detections", response[2]);

	//store distancez in an array
	unsigned short distances[numDetections];
	unsigned short amplitudes[numDetections];
	for(unsigned i = 0; i < numDetections; i++){
		distances[i]	= Detections[i*5] | (Detections[i*5+1] << 8);
		amplitudes[i]	= (Detections[i*5+2] | (Detections[i*5+1+2] << 8))/64;
	}
	for(unsigned i = 0; i < numDetections; i++) {
		SmartDashboard::PutNumber("distance of detection " + to_string(i+1), distances[i]);
		SmartDashboard::PutNumber("amplitude of detection " + to_string(i+1), amplitudes[i]);
	}
}
