/*
 * SuspensionDrive.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#include <SuspensionDrive.h>

SuspensionDrive::SuspensionDrive():
	m_driveFR(0),
	m_driveFL(1),
	m_driveBR(2),
	m_driveBL(3)
{
}

SuspensionDrive::~SuspensionDrive() {
}


void SuspensionDrive::SetFrontLeft(bool val){
	m_driveFL.Set(val);
}

void SuspensionDrive::SetFrontRight(bool val){
	m_driveFR.Set(val);
}

void SuspensionDrive::SetBackLeft(bool val){
	m_driveBL.Set(val);
}

void SuspensionDrive::SetBackRight(bool val){
	m_driveBR.Set(val);
}

void SuspensionDrive::SetLeft(bool val){
	SetBackLeft(val);
	SetFrontLeft(val);
}

void SuspensionDrive::SetRight(bool val){
	SetBackRight(val);
	SetFrontRight(val);
}

void SuspensionDrive::SetFront(bool val){
	SetFrontLeft(val);
	SetFrontRight(val);
}

void SuspensionDrive::SetBack(bool val){
	SetBackLeft(val);
	SetBackRight(val);
}
