/*
 * TankDrive.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#include <DriveTrain/TankDrive.h>

TankDrive::TankDrive() :
		m_leftMotor1(1), m_leftMotor2(2), m_rightMotor1(3), m_rightMotor2(4)
{
	// TODO Auto-generated constructor stub
}

TankDrive::~TankDrive()
{
	// TODO Auto-generated destructor stub
}

void TankDrive::Drive(float leftSpeed, float rightSpeed)
{
	m_leftMotor1.Set(leftSpeed);
	m_leftMotor2.Set(leftSpeed);
	m_rightMotor1.Set(-rightSpeed);
	m_rightMotor2.Set(-rightSpeed);
}

void TankDrive::Stop()
{
	m_leftMotor1.StopMotor();
	m_leftMotor2.StopMotor();
	m_rightMotor1.StopMotor();
	m_rightMotor2.StopMotor();
}
