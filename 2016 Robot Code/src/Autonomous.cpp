/*
 * Autonomous.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: Win7
 */

#include <Autonomous.h>

Autonomous::Autonomous() {
	// TODO Auto-generated constructor stub

}

Autonomous::~Autonomous() {
	// TODO Auto-generated destructor stub
}

void Autonomous::Init(AutoMode mode) {
	m_mode = mode;
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
			m_move.SetAccel(10);
			m_move.SetDecel(10);
			m_move.SetMaxSpeed(20);
			m_move.SetDistance(50);
			m_move.CalcParams();
			break;
	}
}

void Autonomous::Periodic() {
	switch(m_mode){
	case DO_NOTHING:

		break;
	case REACH_DEFENSE:
		m_tank->Drive(m_move.Position(m_autoTime.Get()),
					  m_move.Position(m_autoTime.Get()));
		break;
	}
}
