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
//	m_leftMotor1.SetControlMode(CANSpeedController::kSpeed);
//	m_leftMotor2.SetControlMode(CANSpeedController::kFollower);
//	m_rightMotor1.SetControlMode(CANSpeedController::kSpeed);
//	m_rightMotor2.SetControlMode(CANSpeedController::kFollower);
	m_leftMotor1.SetControlMode(CANSpeedController::kPercentVbus);
	m_leftMotor2.SetControlMode(CANSpeedController::kPercentVbus);
	m_rightMotor1.SetControlMode(CANSpeedController::kPercentVbus);
	m_rightMotor2.SetControlMode(CANSpeedController::kPercentVbus);

	m_leftMotor1.Enable();
	m_leftMotor2.Enable();
	m_rightMotor1.Enable();
	m_rightMotor2.Enable();
//	m_leftMotor1.SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
//	m_leftMotor1.SetSensorDirection(true);
//	m_rightMotor1.SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
//	m_rightMotor1.SetSensorDirection(true);

//	m_leftMotor2.Set(0);
//	m_leftMotor2.SetInverted(true);
//	m_rightMotor2.Set(2);
//	m_rightMotor2.SetInverted(true);
}

TankDrive::~TankDrive()
{
}

void TankDrive::Drive(float leftSpeed, float rightSpeed)
{

	m_leftMotor1.Set(-leftSpeed);
	m_leftMotor2.Set(-leftSpeed);
	m_rightMotor1.Set(rightSpeed);
	m_rightMotor2.Set(rightSpeed);
}

void TankDrive::Stop()
{
	//m_leftMotor1.StopMotor();
	//m_leftMotor2.StopMotor();
	//m_rightMotor1.StopMotor();
	//m_rightMotor2.StopMotor();
}
