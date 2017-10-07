/*
 * Autonomous.h
 *
 *  Created on: Mar 7, 2016
 *      Author: Win7
 */

#ifndef SRC_AUTONOMOUS_AUTONOMOUS_H_
#define SRC_AUTONOMOUS_AUTONOMOUS_H_
#include "TrapezoidalMove.h"
#include "DriveTrain/TankDrive.h"
#include "DriveTrain/SuspensionDrive.h"
#include "Shooter.h"
#include "GoalVision.h"
#include "ADXRS450_Gyro_4967.h"
#include <WPILib.h>

class Autonomous {
private:
	double m_autoStartTime = 0;
	TankDrive *m_tank;
	SuspensionDrive *m_suspension;
	ADXRS450_Gyro *m_rateSensor;
	Shooter *m_shooter;
//	GoalVision *m_goalVision;
	unsigned m_autoState = 0;
	DigitalInput m_DIO0, m_DIO1, m_DIO2, m_DIO3, m_DIO4, m_DIO5;
	bool m_init = false;
	int m_mode = 0, m_pos = 0;
public:
//	Autonomous(TankDrive*, SuspensionDrive*, Shooter*, ADXRS450_Gyro*, GoalVision*);
	Autonomous(TankDrive*, SuspensionDrive*, Shooter*);
	virtual ~Autonomous();
	void Init(int mode);
	void Periodic();
	void DisabledInit();
	void RockWall(int position);
	void LowBar(unsigned position);
	void TwoBallLowBar();
	void Ramparts(int position);
	void RoughTerrain(int position);
	void Portcullis();
	void Moat(int position);
	void ChivalDeFrise(int position);
};

#endif /* SRC_AUTONOMOUS_AUTONOMOUS_H_ */
