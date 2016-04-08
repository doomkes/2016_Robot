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
#include <wpilib.h>
enum AutoMode {
	DO_NOTHING,
	CROSS_DEFENSE,
	LOW_BAR
};

class Autonomous {
private:
	//Timer m_autoTime;
	double m_autoStartTime = 0;
	TankDrive *m_tank;
	SuspensionDrive *m_suspension;
	Shooter *m_shooter;
	unsigned m_autoState = 0;
	ADXRS450_Gyro m_rateSensor;
	DigitalInput m_one, m_two, m_three, m_four, m_five, m_six;
	TrapezoidalMove m_move;
	//bool Cross(Defense, TankDrive*, SuspensionDrive*);
	int m_mode = DO_NOTHING;
public:
	Autonomous(TankDrive*, SuspensionDrive*,Shooter*);
	virtual ~Autonomous();
	void Init(int mode);
	void Periodic();
	void RockWall(unsigned position);
	void LowBar(unsigned position);
	void Ramparts(int position);
};

#endif /* SRC_AUTONOMOUS_AUTONOMOUS_H_ */
