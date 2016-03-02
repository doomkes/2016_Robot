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
	m_leftMotor1.SetControlMode(CANSpeedController::kPercentVbus);
	m_leftMotor2.SetControlMode(CANSpeedController::kFollower);
	m_rightMotor1.SetControlMode(CANSpeedController::kPercentVbus);
	m_rightMotor2.SetControlMode(CANSpeedController::kFollower);

	m_leftMotor1.SetSensorDirection(true);
	m_rightMotor1.SetSensorDirection(true);

	//m_leftMotor1.SetPID(1.0, 0.0 , 0.0);
	//m_rightMotor1.SetPID(1.0, 0.0 , 0.0);

	m_leftMotor2.Set(LEFTDRIVE1);
	m_rightMotor2.Set(RIGHTDRIVE1);

}

TankDrive::~TankDrive()
{
	// TODO Auto-generated destructor stub
}

void TankDrive::Drive(float leftSpeed, float rightSpeed)
{
	//float P = SmartDashboard::GetNumber("Drive P", 0);
	//float I = SmartDashboard::GetNumber("Drive I", 0);
	//float D = SmartDashboard::GetNumber("Drive D", 0);

	//m_leftMotor1.SetPID(P , I , D);
	//m_rightMotor1.SetPID(P , I , D);


	m_leftMotor1.Set(-leftSpeed);
	m_rightMotor1.Set(rightSpeed);

}

void TankDrive::Stop()
{
	m_leftMotor1.Set(0);
	m_rightMotor1.Set(0);
}
