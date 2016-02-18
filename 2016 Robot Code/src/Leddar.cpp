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


vector<Detection> Leddar::GetDetections() {
	vector<Detection> detections;
	m_RS_232.Reset();
	//sending the bytes to the leddar sensor
	char query[] = {0x01, 0x41,0xC0,0x10};
	m_RS_232.Write(query, 4);


	Wait(0.02);
	char Detections[16*5];
	unsigned bytesRecived = m_RS_232.GetBytesReceived();
	SmartDashboard::PutNumber("bytes recived", bytesRecived);

	//saving the data recived back from the leddar
	char response[bytesRecived];
	m_RS_232.Read(response, bytesRecived);
	memcpy(Detections, response+3, 16*5);
	//putting it on the SmartDashboard so that we could see a few of the bytes we were getting back
	SmartDashboard::PutNumber("byte 0 address", response[0]);
	SmartDashboard::PutNumber("byte 1 function code", response[1]);
	SmartDashboard::PutNumber("byte 2 # of detections", response[2]);

	//store distancez in an array
	unsigned short distances[16];
	for(unsigned i = 0; i < 16; i++){
		distances[i] = Detections[i*5] | (Detections[i*5+1] << 8);
	}
	//unsigned short distance1 = response[3] | (response[4] << 8);
	for(unsigned i = 0; i < 16; i++) {
		SmartDashboard::PutNumber("distance of detection " + to_string(i+1), distances[i]);
	}

	return detections;
}
