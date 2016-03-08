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
enum DriveMode {
	SPEED_MODE,
	POSITION_MODE,
	VBUS_MODE,
};
class TankDrive
{
private:
	CANTalon m_leftMotor1, m_leftMotor2, m_rightMotor1, m_rightMotor2;
	DriveMode m_mode = VBUS_MODE;
	float m_maxSpeed;
	bool m_reverse = false;
	float m_leftDistance = 0, m_rightDistance = 0;

	void SpeedDrive(float leftSpeed, float rightSpeed);
	void VBusDrive(float leftSpeed, float rightSpeed);
	void PositionDrive(float leftSpeed, float rightSpeed);
public:
	TankDrive();
	virtual ~TankDrive();
	void Zero();
	void Reverse(bool);

	void SetMode(DriveMode);
	void Drive(float leftSpeed, float rightSpeed);
};

#endif /* SRC_TANKDRIVE_H_ */
