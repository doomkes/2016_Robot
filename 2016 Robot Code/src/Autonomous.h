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
#include <wpilib.h>
enum AutoMode {
	DO_NOTHING,
	REACH_DEFENSE,
};

class Autonomous {
private:
	Timer m_autoTime;

	TankDrive *m_tank;
	SuspensionDrive *m_suspension;
	TrapezoidalMove m_move;
	//bool Cross(Defense, TankDrive*, SuspensionDrive*);
	AutoMode m_mode = DO_NOTHING;
public:
	Autonomous(TankDrive*, SuspensionDrive*);
	virtual ~Autonomous();
	void Init(AutoMode);
	void Periodic();
};

#endif /* SRC_AUTONOMOUS_AUTONOMOUS_H_ */
