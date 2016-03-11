/*
 * Autonomous.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: Win7
 */

#include <Autonomous.h>

Autonomous::Autonomous(TankDrive* tank, SuspensionDrive* suspension,Shooter* shooter)
	: m_tank(tank), m_suspension(suspension), m_shooter(shooter) {
	// TODO Auto-generated constructor stub
}

Autonomous::~Autonomous() {
	// TODO Auto-generated destructor stub
}

void Autonomous::Init(int mode) {
	float totalDistance = SmartDashboard::GetNumber("Total Distance", 284.93);
	m_mode = mode;
	m_tank->Zero();
	m_autoStartTime = Timer::GetFPGATimestamp();
	switch(mode) {
		case 0:
			m_move.SetAccel(0.00001);
			m_move.SetDecel(0.00001);
			m_move.SetMaxSpeed(0.0001);
			m_move.SetDistance(0);
			m_move.CalcParams();
			break;
		case 1:
			m_tank->SetMode(POSITION_MODE);
			m_tank->Zero();
			m_move.SetAccel(12);
			m_move.SetDecel(12);
			m_move.SetMaxSpeed(36);
			m_move.SetDistance(12*14);
			m_move.CalcParams();
			break;

		case 2:
			m_tank->SetMode(POSITION_MODE);
			m_tank->Zero();
			m_move.SetAccel(12);
			m_move.SetDecel(12);
			m_move.SetMaxSpeed(36);
			m_move.SetDistance(12*19);
			m_move.CalcParams();
			break;
		case 3:
			m_tank->SetMode(POSITION_MODE);
			m_tank->Zero();
			m_move.SetAccel(12);
			m_move.SetDecel(12);
			m_move.SetMaxSpeed(36);
			m_move.SetDistance(totalDistance);
			m_move.CalcParams();
			break;
	}
}

void Autonomous::Periodic() {
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;
	float leftDist, rightDist;
	float  curveRatio = SmartDashboard::GetNumber("Curve Ratio", 0.849084);

	static float autoCount = 0;
	switch(m_mode){
		case 0:
			SmartDashboard::PutString("Auto Mode", "do nothing");
			break;
		case 1:
			m_suspension->SetFrontLeft(true);
			m_suspension->SetBackLeft(true);
			m_suspension->SetFrontRight(true);
			m_suspension->SetBackRight(true);
			SmartDashboard::PutString("Auto Mode", "Cross Defense");
			SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
			leftDist = m_move.Position(currentAutoTime);
			rightDist = m_move.Position(currentAutoTime);
			SmartDashboard::PutNumber("Auto Left distance", leftDist);
			SmartDashboard::PutNumber("Auto Right distance", rightDist);
			SmartDashboard::PutNumber("Auto Count", autoCount);
			m_tank->PositionDrive(leftDist, rightDist);
			autoCount++;
			break;
		case 2:
			m_shooter->LiftTo(180);
			SmartDashboard::PutString("Auto Mode", "Low Bar Defense");
			SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
			leftDist = m_move.Position(currentAutoTime);
			rightDist = m_move.Position(currentAutoTime);
			SmartDashboard::PutNumber("Auto Left distance", leftDist);
			SmartDashboard::PutNumber("Auto Right distance", rightDist);
			SmartDashboard::PutNumber("Auto Count", autoCount);
			m_tank->PositionDrive(leftDist, rightDist);
			autoCount++;
			if (currentAutoTime > m_move.GetTotalTime())
				m_shooter->LiftTo(0);

			break;
		case 3:
			m_shooter->LiftTo(180);
			SmartDashboard::PutString("Auto Mode", "Low Bar Defense");
			SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
			rightDist = m_move.Position(currentAutoTime);
			//rightDist = m_move.Position(currentAutoTime);
			SmartDashboard::PutNumber("Auto Left distance", leftDist);
			SmartDashboard::PutNumber("Auto Right distance", rightDist);
			SmartDashboard::PutNumber("Auto Count", autoCount);

			autoCount++;
			if (rightDist < 146.84){
				//leftDist = m_move.Position(currentAutoTime);
				leftDist = m_move.Position(currentAutoTime);
			}
			if (rightDist >= 146.84){
				//leftDist = m_move.Position(currentAutoTime);
				leftDist = 146.84 + (rightDist - 146.84) *curveRatio;
				m_shooter->LiftTo(42);
			}
			if ((rightDist >= 200) && (rightDist <= 240)){
				m_shooter->Spinup(-8);
						}
			m_tank->PositionDrive(leftDist, rightDist);
			if (currentAutoTime > m_move.GetTotalTime() && currentAutoTime < (m_move.GetTotalTime() + 5))
				//m_shooter->Shoot(true);
				m_shooter->Spinup(12);
			if (currentAutoTime > (m_move.GetTotalTime() + 3))
				m_shooter->Shoot(true);
			if (currentAutoTime > (m_move.GetTotalTime() + 5))
				m_shooter->Spinup(0);
			break;

	}
}
