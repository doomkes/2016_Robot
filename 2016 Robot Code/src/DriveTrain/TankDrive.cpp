/*
 * TankDrive.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#include <DriveTrain/TankDrive.h>
#include "robotmap.h"
using namespace std;
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

	m_leftMotor1.SetSensorDirection(true);
	m_rightMotor1.SetSensorDirection(true);

	m_leftMotor1.SetPosition(0);
	m_rightMotor1.SetPosition(0);
	m_leftMotor2.Set(LEFTDRIVE1);
	m_rightMotor2.Set(RIGHTDRIVE1);

	m_leftMotor1.SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	m_rightMotor1.SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
}

TankDrive::~TankDrive()
{
}

void TankDrive::SetMode(DriveMode mode) {
	Zero();
	m_mode = mode;
	m_leftMotor1.SetPID(SmartDashboard::GetNumber("Driver P", 1),  SmartDashboard::GetNumber("Driver I", 0), 0);
	m_rightMotor1.SetPID(SmartDashboard::GetNumber("Driver P", 1), SmartDashboard::GetNumber("Driver I", 0), 0);
	switch(mode) {
		case SPEED_MODE:
			m_leftMotor1.SetControlMode(CANSpeedController::kPosition);
			m_leftMotor1.Set(0);
			m_rightMotor1.SetControlMode(CANSpeedController::kPosition);
			m_rightMotor1.Set(0);
			break;
		case VBUS_MODE:
			m_leftMotor1.SetControlMode(CANSpeedController::kPercentVbus);
			m_leftMotor1.Set(0);
			m_rightMotor1.SetControlMode(CANSpeedController::kPercentVbus);
			m_rightMotor1.Set(0);
			break;
		case POSITION_MODE:
			m_leftMotor1.SetControlMode(CANSpeedController::kPosition);
			m_leftMotor1.Set(0);
			m_rightMotor1.SetControlMode(CANSpeedController::kPosition);
			m_rightMotor1.Set(0);
			break;
	}
}

void TankDrive::SpeedDrive(float leftSpeed, float rightSpeed) {
	static Timer timer;
	float dt = timer.Get();

	if(dt > 0.025) {
		dt = 0.025;
	}

	m_leftDistance += leftSpeed*40*dt;
	m_rightDistance += rightSpeed*40*dt;

	m_leftMotor1.Set(-m_leftDistance*COUNT_PER_INCH);
	m_rightMotor1.Set(m_rightDistance*COUNT_PER_INCH);

	timer.Reset();
	timer.Start();
}

void TankDrive::PositionDrive(float leftDist, float rightDist) {
//	SmartDashboard::PutNumber("left drive target pos", leftSpeed*COUNT_PER_INCH);
//	SmartDashboard::PutNumber("left drive pos", m_leftMotor1.Get());
	m_leftDistance = leftDist;
	m_rightDistance = rightDist;
	m_leftMotor1.Set(-m_leftDistance*COUNT_PER_INCH);
	m_rightMotor1.Set(m_rightDistance*COUNT_PER_INCH);

	SmartDashboard::PutNumber("left  Error", m_leftMotor1.GetClosedLoopError());
	SmartDashboard::PutNumber("right Error", m_rightMotor1.GetClosedLoopError());

}

void TankDrive::VBusDrive(float leftSpeed, float rightSpeed) {
	m_leftMotor1.Set(-leftSpeed);
	m_rightMotor1.Set(rightSpeed);
}
void TankDrive::StraightDrive(float dist, float angleError, bool reset ) {
	static float adjust = 0;
	if(reset) {
		adjust = 0;
	}
	//0.367 is the calculated angle adjust factor
	//0.05 is the dampener
	adjust += angleError * 0.367 * 0.05;
	m_leftMotor1.Set((dist + adjust/2)*COUNT_PER_INCH);
	m_rightMotor1.Set(-(dist - adjust/2)*COUNT_PER_INCH);

}
void TankDrive::Drive(float leftSpeed, float rightSpeed)
{
	if(fabs(leftSpeed) < 0.1) {
		leftSpeed = 0;
	}
	if(fabs(rightSpeed) < 0.1) {
		rightSpeed = 0;
	}

	if(m_reverse) {
		leftSpeed = -leftSpeed;
		rightSpeed = -rightSpeed;
		swap(leftSpeed, rightSpeed);
	}

	switch(m_mode) {
		case SPEED_MODE:
			SpeedDrive(leftSpeed, rightSpeed);
			break;
		case VBUS_MODE:
			VBusDrive(leftSpeed, rightSpeed);
			break;
		case POSITION_MODE:
			PositionDrive(leftSpeed, rightSpeed);
			break;
	}
	SmartDashboard::PutNumber("left dist", m_leftDistance);
	SmartDashboard::PutNumber("right dist", m_rightDistance);
	SmartDashboard::PutNumber("left drive error", m_leftDistance*COUNT_PER_INCH - m_leftMotor1.Get());
	SmartDashboard::PutNumber("right drive error", m_rightDistance*COUNT_PER_INCH - m_rightMotor1.Get());
}

void TankDrive::Reverse(bool reverse) {
	m_reverse = reverse;
}
void TankDrive::Zero()
{
	m_leftMotor1.SetPosition(0.0);
	m_rightMotor1.SetPosition(0.0);
	m_leftMotor1.Set(0);
	m_rightMotor1.Set(0);
	m_leftDistance = 0;
	m_rightDistance = 0;
}

