/*
 * SuspensionDrive.h
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#ifndef SRC_SUSPENSIONDRIVE_H_
#define SRC_SUSPENSIONDRIVE_H_

#include <WPILib.h>

class SuspensionDrive
{
private:
	Solenoid m_driveFR, m_driveFL, m_driveBR, m_driveBL;
public:
	SuspensionDrive();
	virtual ~SuspensionDrive();

	void SetFrontLeft(bool val);
	void SetFrontRight(bool val);
	void SetBackLeft(bool val);
	void SetBackRight(bool val);

	void SetLeft(bool val);
	void SetRight(bool val);
	void SetFront(bool val);
	void SetBack(bool val);
};

#endif /* SRC_SUSPENSIONDRIVE_H_ */
