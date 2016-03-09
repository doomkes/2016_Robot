/*
 * Autonomous.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: Win7
 */

#include <Autonomous.h>

Autonomous::Autonomous(TankDrive* tank, SuspensionDrive* suspension)
	: m_tank(tank), m_suspension(suspension) {
	// TODO Auto-generated constructor stub
}

Autonomous::~Autonomous() {
	// TODO Auto-generated destructor stub
}

void Autonomous::Init(AutoMode mode) {
	m_mode = mode;
	m_tank->Zero();
	m_autoTime.Reset();
	m_autoTime.Start();
	switch(mode) {
		case DO_NOTHING:
			m_move.SetAccel(0.00001);
			m_move.SetDecel(0.00001);
			m_move.SetMaxSpeed(0.0001);
			m_move.SetDistance(0);
			m_move.CalcParams();
			break;
		case REACH_DEFENSE:
			m_tank->SetMode(POSITION_MODE);
			m_tank->Zero();
			m_move.SetAccel(0.5);
			m_move.SetDecel(0.5);
			m_move.SetMaxSpeed(1);
			m_move.SetDistance(5);
			m_move.CalcParams();
			break;
	}
}

void Autonomous::Periodic() {
	if(m_autoTime.Get() >= 2 || m_autoTime.Get() > m_move.GetTotalTime()) {
		return;
	}
	switch(m_mode){
	case DO_NOTHING:
		SmartDashboard::PutString("Auto Mode", "do nothing");

		break;
	case REACH_DEFENSE:
		SmartDashboard::PutString("Auto Mode", "Reach Defense");
		m_tank->Drive(m_move.Position(m_autoTime.Get()),
					  m_move.Position(m_autoTime.Get()));
		break;
	}
}
