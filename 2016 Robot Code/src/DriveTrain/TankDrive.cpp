/*
 * TankDrive.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#include <DriveTrain/TankDrive.h>
#include "robotmap.h"
TankDrive::TankDrive() :
		m_leftMotor1(LEFTDRIVE1),
		m_leftMotor2(LEFTDRIVE2),
		m_rightMotor1(RIGHTDRIVE1),
		m_rightMotor2(RIGHTDRIVE2)
{
	m_leftMotor1.SetControlMode(CANSpeedController::kFollower);
	m_leftMotor2.SetControlMode(CANSpeedController::kSpeed);
	m_rightMotor1.SetControlMode(CANSpeedController::kFollower);
	m_rightMotor2.SetControlMode(CANSpeedController::kSpeed);

	m_leftMotor1.Set(LEFTDRIVE2);
	m_rightMotor1.Set(RIGHTDRIVE2);
	// TODO TOT-generated constructor stub
}

TankDrive::~TankDrive()
{
	// TODO Auto-generated destructor stub
}

void TankDrive::Drive(float leftSpeed, float rightSpeed)
{
	//m_leftMotor1.Set(-leftSpeed);
	m_leftMotor2.Set(-leftSpeed);
	//m_rightMotor1.Set(rightSpeed);
	m_rightMotor2.Set(rightSpeed);
}

void TankDrive::Stop()
{
	//m_leftMotor1.StopMotor();
	m_leftMotor2.StopMotor();
	//m_rightMotor1.StopMotor();
	m_rightMotor2.StopMotor();
}
