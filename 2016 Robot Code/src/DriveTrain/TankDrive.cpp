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

	m_leftMotor1.SetControlMode(CANSpeedController::kPosition);
	m_leftMotor2.SetControlMode(CANSpeedController::kFollower);
	m_rightMotor1.SetControlMode(CANSpeedController::kPosition);
	m_rightMotor2.SetControlMode(CANSpeedController::kFollower);

	//m_leftMotor1.SetVoltageRampRate(6);
	//m_rightMotor1.SetVoltageRampRate(6);

	m_leftMotor1.SetSensorDirection(true);
	m_rightMotor1.SetSensorDirection(true);

	m_leftMotor1.SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	m_rightMotor1.SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);

	m_leftMotor1.SetPosition(0);
	m_rightMotor1.SetPosition(0);
	m_leftMotor2.Set(LEFTDRIVE1);
	m_rightMotor2.Set(RIGHTDRIVE1);

	m_leftMotor1.Enable();
	m_leftMotor2.Enable();
	m_rightMotor1.Enable();
	m_rightMotor2.Enable();
}

TankDrive::~TankDrive()
{
}

void TankDrive::Drive(float leftSpeed, float rightSpeed)
{

	static Timer timer;
	static float leftDistance = 0, rightDistance = 0;

	if(fabs(leftSpeed) < 0.1) {
		leftSpeed = 0;
	}
	if(fabs(rightSpeed) < 0.1) {
		rightSpeed = 0;
	}

	m_leftMotor1.SetPID(SmartDashboard::GetNumber("Driver P", 0), 0, 0);
	m_rightMotor1.SetPID(SmartDashboard::GetNumber("Driver P", 0), 0, 0);

	float dt = timer.Get();

	if(dt > 0.025) {
		dt = 0.025;
	}

	leftDistance += leftSpeed*10*dt;
	rightDistance += rightSpeed*10*dt;
	m_leftMotor1.Set(-leftDistance*COUNT_PER_INCH);
	m_rightMotor1.Set(rightDistance*COUNT_PER_INCH);

	SmartDashboard::PutNumber("left dist", leftDistance);
	SmartDashboard::PutNumber("right dist", rightDistance);

	timer.Reset();
	timer.Start();
}

void TankDrive::Zero()
{
	m_leftMotor1.SetPosition(0.0);
	m_rightMotor1.SetPosition(0.0);
}

void TankDrive::Stop()
{
	//m_leftMotor1.StopMotor();
	//m_leftMotor2.StopMotor();
	//m_rightMotor1.StopMotor();
	//m_rightMotor2.StopMotor();
}
