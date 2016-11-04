/*
 * TankDrive.h
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#ifndef SRC_TANKDRIVE_H_
#define SRC_TANKDRIVE_H_

#include <wpilib.h>
#include "CANTalon.h"
#include "RobotMap.h"

class TankDrive
{
private:
	CANTalon m_leftMotor1, m_leftMotor2, m_rightMotor1, m_rightMotor2;
public:
	TankDrive() :
			m_leftMotor1(LEFTDRIVE1), m_leftMotor2(LEFTDRIVE2), m_rightMotor1(
					RIGHTDRIVE1), m_rightMotor2(RIGHTDRIVE2) {
		m_leftMotor1.SetControlMode(CANSpeedController::kFollower);
		m_leftMotor2.SetControlMode(CANSpeedController::kSpeed);
		m_rightMotor1.SetControlMode(CANSpeedController::kFollower);
		m_rightMotor2.SetControlMode(CANSpeedController::kSpeed);
		m_leftMotor1.Set(LEFTDRIVE2);
		m_rightMotor1.Set(RIGHTDRIVE2);
		// TODO TOT-generated constructor stub
	}
	virtual ~TankDrive();

	void Drive(float leftSpeed, float rightSpeed);
	void Stop();
};

#endif /* SRC_TANKDRIVE_H_ */
