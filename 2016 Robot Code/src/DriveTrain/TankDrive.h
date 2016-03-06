/*
 * TankDrive.h
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#ifndef SRC_TANKDRIVE_H_
#define SRC_TANKDRIVE_H_

#include <wpilib.h>
#define DIST_PER_REV (6*3.141592)
#define COUNT_PER_INCH (1/DIST_PER_REV)
class TankDrive
{
private:
	CANTalon m_leftMotor1, m_leftMotor2, m_rightMotor1, m_rightMotor2;
	float m_maxSpeed;
public:
	TankDrive();
	virtual ~TankDrive();
	void Zero();

	void Drive(float leftSpeed, float rightSpeed);
	void Stop();
};

#endif /* SRC_TANKDRIVE_H_ */
