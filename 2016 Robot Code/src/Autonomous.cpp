/*
 * Autonomous.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: Win7
 */

#include <Autonomous.h>

Autonomous::Autonomous(TankDrive* tank, SuspensionDrive* suspension,Shooter* shooter)
	: m_tank(tank), m_suspension(suspension), m_shooter(shooter), m_one(1), m_two(2), m_three(3), m_four(4) {
	// TODO Auto-generated constructor stub
}

Autonomous::~Autonomous() {
	// TODO Auto-generated destructor stub
}

void Autonomous::Init(int mode) {
	float totalDistance = SmartDashboard::GetNumber("Total Distance", 284.93);
	m_mode = 0;
	if (!m_one.Get()) m_mode = 1;
	else if (!m_two.Get()) m_mode = 2;
	else if (!m_three.Get()) m_mode = 3;
	else if (!m_four.Get()) m_mode = 4;
	m_tank->Zero();
	m_autoStartTime = Timer::GetFPGATimestamp();
	switch(m_mode) {
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
		case 4:
			m_tank->SetMode(POSITION_MODE);
			m_tank->Zero();
			m_move.SetAccel(24);
			m_move.SetDecel(24);
			m_move.SetMaxSpeed(64);
			m_move.SetDistance(totalDistance);
			m_move.CalcParams();
			break;
	}
}

void Autonomous::Periodic() {
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;
	float leftDist, rightDist;
	float  curveRatio = 0.78;

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
		case 4:
			SmartDashboard::PutString("Auto Mode", "Low Bar Defense");
			SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
			rightDist = m_move.Position(currentAutoTime);
			SmartDashboard::PutNumber("Auto Left distance", leftDist);
			SmartDashboard::PutNumber("Auto Right distance", rightDist);
			SmartDashboard::PutNumber("Auto Count", autoCount);
			if (currentAutoTime < m_move.GetTotalTime())	autoCount++;
			if (currentAutoTime > (m_move.GetTotalTime() + 2))	autoCount--;	//run trapizoidal move backwards?
			if (rightDist < 146.84){	//get under low bar
				leftDist = m_move.Position(currentAutoTime);
				m_shooter->LiftTo(180);
			}
			if (rightDist >= 146.84){	//begin curve and raise shooter
				leftDist = 146.84 + (rightDist - 146.84) *curveRatio;
				m_shooter->LiftTo(42);
			}
			if ((rightDist >= 200) && (rightDist <= 220)){	//make sure the ball is sucked in
				m_shooter->Spinup(-8);
						}
			m_tank->PositionDrive(leftDist, rightDist);	//spin up wheels until ball is shot
			if ((rightDist >= 220) && currentAutoTime < (m_move.GetTotalTime() + 2))
				m_shooter->Spinup(12);
			if (currentAutoTime > (m_move.GetTotalTime() + 1))	//wait for a sec to make sure we are lined up
				m_shooter->Shoot(true);
			if ((currentAutoTime > (m_move.GetTotalTime() + 2)) && (currentAutoTime > (m_move.GetTotalTime() + 2.1))) {	//Give the ball time to clear the robot, then lower shooter and de-spin wheels.
				m_shooter->Spinup(0);
				m_shooter->LiftTo(0);
			}
			break;
	}
}
