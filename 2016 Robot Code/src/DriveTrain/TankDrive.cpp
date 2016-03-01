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
	m_leftMotor1.SetPID(1.0, 0.0 , 0.0);
	m_leftMotor2.SetPID(1.0, 0.0 , 0.0);
	m_rightMotor1.SetPID(1.0, 0.0 , 0.0);
	m_rightMotor2.SetPID(1.0, 0.0 , 0.0);
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
	m_leftMotor2.Set(-leftSpeed);
	m_rightMotor2.Set(rightSpeed);
}

void TankDrive::Stop()
{
	m_leftMotor2.Set(0);
	m_rightMotor2.Set(0);
}
