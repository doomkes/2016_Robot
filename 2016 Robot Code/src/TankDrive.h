/*
 * TankDrive.h
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#ifndef SRC_TANKDRIVE_H_
#define SRC_TANKDRIVE_H_

#include <wpilib.h>

class TankDrive {
private:
	CANTalon m_leftMotor1, m_leftMotor2, m_rightMotor1, m_rightMotor2;
public:
	TankDrive();
	virtual ~TankDrive();

	void Drive(float leftSpeed, float rightSpeed);
	void Stop();
};

#endif /* SRC_TANKDRIVE_H_ */
