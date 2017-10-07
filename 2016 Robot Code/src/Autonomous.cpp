/*
 * Autonomous.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: Win7
 */

#include <Autonomous.h>

//Autonomous::Autonomous(TankDrive* tank, SuspensionDrive* suspension,Shooter* shooter, ADXRS450_Gyro *rateSensor, GoalVision *goalVision)
//	: m_tank(tank), m_suspension(suspension), m_rateSensor(rateSensor),
//	  m_shooter(shooter),m_goalVision(goalVision), m_DIO0(0), m_DIO1(1), m_DIO2(2), m_DIO3(3), m_DIO4(4), m_DIO5(5) {
//	// TODO Auto-generated constructor stub
//}
Autonomous::Autonomous(TankDrive* tank, SuspensionDrive* suspension,Shooter* shooter)
	: m_tank(tank), m_suspension(suspension),
	  m_shooter(shooter), m_DIO0(0), m_DIO1(1), m_DIO2(2), m_DIO3(3), m_DIO4(4), m_DIO5(5) {
	// TODO Auto-generated constructor stub
}

Autonomous::~Autonomous() {
	// TODO Auto-generated destructor stub
}

void Autonomous::Init(int mode) {
	m_mode = 0;
	m_pos = 0;
	m_mode |= !m_DIO0.Get() ? 0x04 : 0;
	m_mode |= !m_DIO1.Get() ? 0x02 : 0;
	m_mode |= !m_DIO2.Get() ? 0x01 : 0;

	m_pos |= !m_DIO3.Get() ? 0x02 : 0;
	m_pos |= !m_DIO4.Get() ? 0x01 : 0;

	SmartDashboard::PutNumber("auto mode", m_mode);
	SmartDashboard::PutNumber("auto pos", m_pos);
	m_tank->Zero();
	m_autoState = 0;
	m_autoStartTime = Timer::GetFPGATimestamp();
	m_tank->SetMode(POSITION_MODE);

	//textual representation of selected auto mode.
	std::string autoName = "";
	switch(m_mode) {
		case 0: autoName = "do nothing"; break;
		case 1: autoName = "low bar"; break;
		case 2: autoName = "portcullis"; break;
		case 3: autoName = "Chival De Friese"; break;
		case 4: autoName = "Ramparts"; break;
		case 5: autoName = "Moat"; break;
		case 6: autoName = "Rock Wall"; break;
		case 7: autoName = "RoughTerrain"; break;
	}
	//Put textual representation of auto mode to dashboard.
	SmartDashboard::PutString("auto name", std::to_string(m_mode) + ": " + autoName);
	m_init = true;
}

void Autonomous::Periodic() {
	static unsigned count = 0;
	count++;
	if (count % 50 == 0)
		SmartDashboard::PutNumber("RS Angle", m_rateSensor->GetAngle());

	switch(m_mode){
		case 0:
			break;
		case 1:
			LowBar(0);
			//TwoBallLowBar();
			break;
		case 2:
			Portcullis();
			break;
		case 3:
			ChivalDeFrise(m_pos);
			break;
		case 4:
			Ramparts(m_pos);
			break;
		case 5:
			Moat(m_pos);
			break;
		case 6:
			RockWall(m_pos);
			break;
		case 7:
			RoughTerrain(m_pos);
			break;

	}
}

void Autonomous::LowBar(unsigned position){ //This now uses vision
	static unsigned count = 0; // General Purpose counter
	static float startAngle = 0;
	static float pos = 0;

	static double caseStartTime = 0;
	static TrapezoidalMove move;
	float leftDist,rightDist;
	static float correctionAngle = 0;
	static float pixelError = 0;
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;

	if(m_init) { // Initilize static variables.
		pos = 0;
		caseStartTime = currentAutoTime;
		move.SetAll(24,24,72,221);

//		Point start, end;

//		start.x = Preferences::GetInstance()->GetInt("Auto left Goal x1", 0);
//		start.y = Preferences::GetInstance()->GetInt("Auto left Goal y1", 0);
//		end.x = Preferences::GetInstance()->GetInt("Auto left Goal x2", 0);
//		end.y = Preferences::GetInstance()->GetInt("Auto left Goal y2", 0);

		correctionAngle = 0;
//		m_goalVision->SetLine(start, end);

//		startAngle = m_rateSensor->GetAngle();
		m_tank->StraightDrive(0, 0, false);
		m_init = false;
	}
	// debugging stop at state...
	//if (m_autoState == 1) m_autoState = 99;

	switch (m_autoState){
		case 0:  // Drive to low bar
			leftDist = move.Position(currentAutoTime);
			rightDist = leftDist;
//			m_tank->StraightDrive(-move.Position(currentAutoTime),
//								  startAngle - m_rateSensor->GetAngle());
			m_tank->Drive(leftDist, rightDist);
			if (rightDist > 200) m_shooter->LiftTo(40);
			else m_shooter->LiftTo(165);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()) {
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_tank->Zero();
			}

			break;
		case 1: // Turn 60 degrees - wheels equal and opposite directions 1/4 of circle circumference

			pos += 0.2;
			m_tank->PositionDrive(-pos, pos);
			if(m_rateSensor->GetAngle() - startAngle > 57) {
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				move.SetAll(24,24,72,40); // Setup move for next step
				m_tank->Zero();
				m_shooter->Spinup(-2000); // set shooter angle and speed set
				m_shooter->LiftTo(40);
			}
			break;
		case 2: // Move toward the tower and bring up shooter
			leftDist = move.Position(currentAutoTime - caseStartTime);
			rightDist = leftDist;
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_shooter->Spinup(5000);
//				startAngle = m_rateSensor->GetAngle();
//				pixelError = m_goalVision->GetAngleCorrection();
				pos = 0;
				m_tank->Zero();
			}
			break;
		case 3: // adjust angle using vision
			correctionAngle = pixelError*.18;
			pos += 0.2*(correctionAngle - (m_rateSensor->GetAngle() - startAngle))/25;
			m_tank->PositionDrive(-pos, pos);
			if(fabs(correctionAngle - (m_rateSensor->GetAngle() - startAngle)) < 0.5) {
				caseStartTime = currentAutoTime;
				m_autoState++;
				m_tank->Zero();
			}
			break;
		case 4: // wait for spinup
			if ((currentAutoTime - caseStartTime) > 2.0){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
			}
			break;
		case 5: // shoot
			m_shooter->Shoot(true);
			if ((currentAutoTime - caseStartTime) > 0.10){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_shooter->Spinup(0);
			}
			break;
	}


	if (count%10 == 0){
		SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
		SmartDashboard::PutNumber("Auto Left distance", leftDist);
		SmartDashboard::PutNumber("Auto Right distance", rightDist);
	}
	count ++; // increment counter
}

void Autonomous::Ramparts(int position){ //This now uses vision

	static unsigned count = 0; // General Purpose counter
	static float adjust = 0;
	static double caseStartTime = 0;
	float leftDist,rightDist;
	static unsigned lastState = 99; // used to detect state change
	static float startAngle = 0;
	static float timeAdjust = 0;
	static TrapezoidalMove move;
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;
	static int finalMoveDist = 0;
	static float correctionAngle = 0;
	static float pixelError = 0;

	if(m_init) { // Initilize static variables.
		switch(position){//set distance of final move based on starting position
			case 0: finalMoveDist = 84; break;
			case 1: finalMoveDist = 31; break;
			case 2: finalMoveDist = 17; break;
			case 3: finalMoveDist = 75; break;
		}
		lastState = 0;
		count = 0;
		caseStartTime = currentAutoTime;
		move.SetAll(24,24,36,220);
		startAngle = m_rateSensor->GetAngle();
		timeAdjust = 0;
		m_tank->StraightDrive(0, 0, false);
		m_suspension->SetFrontLeft(true);
		m_suspension->SetBackLeft(true);
		m_suspension->SetFrontRight(true);
		m_suspension->SetBackRight(true);
		m_init = false;
	}

	leftDist = move.Position(currentAutoTime) + adjust/2;
	rightDist = move.Position(currentAutoTime) - adjust/2;
	printf("case: %i\n",m_autoState);

	switch (m_autoState){
		case 0:  // Drive to ramparts
			if (m_autoState != lastState){ // put things here that only need done once when entering the state

				m_shooter->Spinup(0);
				m_shooter->LiftTo(0);
			}
			adjust += m_rateSensor->GetAngle()*0.367*0.05;
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist > 61.67) m_autoState++;
			break;
		case 1:  // Drive to ramparts
			m_suspension->SetBackLeft(false);
			adjust += m_rateSensor->GetAngle()*0.367*0.05;
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist > 72.795) m_autoState++;
			break;
		case 2:  // Drive to ramparts
			m_suspension->SetBackLeft(true);
			m_suspension->SetBackRight(false);
			adjust += m_rateSensor->GetAngle()*0.367*0.05;
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist > 83.92) m_autoState++;
			break;
		case 3:  // Drive to ramparts
			m_suspension->SetFrontLeft(false);
			adjust += m_rateSensor->GetAngle()*0.367*0.05;
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist > 84.25) m_autoState++;
			break;
		case 4:  // Drive to ramparts
			m_suspension->SetBackRight(true);
			adjust += m_rateSensor->GetAngle()*0.367*0.05;
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist > 98.25) m_autoState++;
			break;
		case 5:  // Drive to ramparts
			m_suspension->SetFrontLeft(true);
			m_suspension->SetFrontRight(false);
			adjust += m_rateSensor->GetAngle()*0.367*0.05;
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist > 105.5) m_autoState++;
			break;
		case 6:  // Drive to ramparts
			m_suspension->SetFrontRight(true);
			adjust += m_rateSensor->GetAngle()*0.367*0.05;
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist > 115) {
				//caseStartTime = currentAutoTime;
				m_autoState++;
			}
			break;
		case 7:
			timeAdjust += 0.01;
			currentAutoTime += timeAdjust;
			adjust += m_rateSensor->GetAngle()*0.367*0.05; //this is the tangent of 1deg times wheel separation of 21in
			m_suspension->SetFrontLeft(false);
			m_suspension->SetBackLeft(false);
			m_suspension->SetFrontRight(false);
			m_suspension->SetBackRight(false);
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_tank->Zero();
				startAngle = m_rateSensor->GetAngle();
				}
			break;
		case 8://first 90deg turn to line up horizontally to tower
			static float pos = 0;
			m_shooter->Spinup(-3000);
			switch (position){
				case 0 ... 1://positions 1 and 2 (turn clockwise)
					pos += 0.2*(90 - m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(-pos, pos);
					if(m_rateSensor->GetAngle() - startAngle > 90) {
						m_autoState++; // move to next state
						caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
						move.SetAll(36,36,48,finalMoveDist); // Setup move for next step
						m_tank->Zero();
					}
					break;
				case 2 ... 3://positions 3 and 4 (turn counter-clockwise)
					pos += 0.2*fabs(90 + m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(pos, -pos);
					if(fabs(m_rateSensor->GetAngle()) + startAngle > 90) {
						m_autoState++; // move to next state
						caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
						move.SetAll(36,36,48,finalMoveDist); // Setup move for next step
						m_tank->Zero();
					}
					break;
			}
			break;
		case 9:
			m_shooter->Spinup(4500);
			m_shooter->LiftTo(45);
			leftDist = move.Position(currentAutoTime - caseStartTime);
			rightDist = leftDist;
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_tank->Zero();
				m_autoState++; // move to next state
				//caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
			}
			break;
		case 10:{//second 90deg turn to line up towards goal
			static float pos = 0;
			m_shooter->Spinup(4500);
			m_shooter->LiftTo(45);
			switch(position){
				case 0 ... 1://positions 1 and 2 (turn counter-clockwise)
					pos += 0.20*(m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(pos, -pos);
					if(m_rateSensor->GetAngle() < 0) {
						m_tank->Zero();
						m_autoState++;
//						startAngle = m_rateSensor->GetAngle();
//						pixelError = m_goalVision->GetAngleCorrection();
						pos = 0;
					}
					break;
				case 2 ... 3://positions 3 and 4 (turn clockwise)
					pos += 0.20*fabs(m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(-pos, pos);
					if(m_rateSensor->GetAngle() > 0) {
						m_tank->Zero();
						m_autoState++;
//						startAngle = m_rateSensor->GetAngle();
//						pixelError = m_goalVision->GetAngleCorrection();
						pos = 0;
					}
					break;
			}
			break;
		}
		case 11: //adjust angle with vision
			correctionAngle = pixelError*.18;
			pos += 0.2*(correctionAngle - (m_rateSensor->GetAngle() - startAngle))/25;
			m_tank->PositionDrive(-pos, pos);

			if(fabs(correctionAngle - (m_rateSensor->GetAngle() - startAngle)) < 0.5) {
				caseStartTime = currentAutoTime;
				m_autoState++;
				m_shooter->Shoot(true);
			}
			break;
		case 12:
			m_tank->Zero();
			m_shooter->LiftTo(45);
			m_shooter->Spinup(0);
			break;
	}


	if (count%10 == 0){
		SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
		SmartDashboard::PutNumber("Auto Left distance", leftDist);
		SmartDashboard::PutNumber("Auto Right distance", rightDist);
	}
	count ++; // increment counter
	lastState = m_autoState;

}

void Autonomous::RoughTerrain(int position){ //This now uses vision
	static unsigned count = 0; // General Purpose counter
	static float adjust = 0;
	static double caseStartTime = 0;
	float leftDist,rightDist;
	static unsigned lastState = 99; // used to detect state change
	static float startAngle = 0;
	static float timeAdjust = 0;
	static TrapezoidalMove move(24,24,36,212);
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;
	static int finalMoveDist = 0;
	static float correctionAngle = 0;
	static float pixelError = 0;

	if(m_init) { // Initilize static variables.
		switch(position){//set distance of final move based on starting position
		case 0: finalMoveDist = 84; break;
		case 1: finalMoveDist = 31; break;
		case 2: finalMoveDist = 22; break;
		case 3: finalMoveDist = 75; break;
			default: finalMoveDist = 0; break;
		}
		lastState = 0;
		count = 0;
		caseStartTime = currentAutoTime;
		move.SetAll(24,24,36,212);
		startAngle = m_rateSensor->GetAngle();
		timeAdjust = 0;
		m_tank->StraightDrive(0, 0, false);
		m_suspension->SetFrontLeft(true);
		m_suspension->SetBackLeft(true);
		m_suspension->SetFrontRight(true);
		m_suspension->SetBackRight(true);
		m_init = false;
	}
	leftDist = move.Position(currentAutoTime) + adjust/2;
	rightDist = move.Position(currentAutoTime) - adjust/2;

	switch (m_autoState){
		case 0:  // Drive over rough terrain with wheels extended
			if (m_autoState != lastState){ // put things here that only need done once when entering the state

				m_shooter->Spinup(0);
				m_shooter->LiftTo(0);
			}
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist > 115) m_autoState++;
			break;
		case 1://finish the rest of the move to the tower with wheels retracted and gyro feedback
			timeAdjust += 0.01;
			currentAutoTime += timeAdjust;
			adjust += m_rateSensor->GetAngle()*0.367*0.05; //this is the tangent of 1deg times wheel separation of 21in
			m_suspension->SetFrontLeft(false);
			m_suspension->SetBackLeft(false);
			m_suspension->SetFrontRight(false);
			m_suspension->SetBackRight(false);
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_tank->Zero();
				startAngle = m_rateSensor->GetAngle();
				}
			break;
		case 2://first 90deg turn to line up horizontally to tower
			static float pos = 0;
			m_shooter->Spinup(-3000);
			switch (position){
				case 0 ... 1://positions 1 and 2 (turn clockwise)
					pos += 0.2*(90 - m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(-pos, pos);
					if(m_rateSensor->GetAngle() - startAngle > 90) {
						m_autoState++; // move to next state
						caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
						move.SetAll(36,36,48,finalMoveDist); // Setup move for next step
						m_tank->Zero();
					}
					break;
				case 2 ... 3://positions 3 and 4 (turn counter-clockwise)
					pos += 0.2*fabs(90 + m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(pos, -pos);
					if(fabs(m_rateSensor->GetAngle()) + startAngle > 90) {
						m_autoState++; // move to next state
						caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
						move.SetAll(36,36,48,finalMoveDist); // Setup move for next step
						m_tank->Zero();
					}
					break;
			}
			break;
		case 3:
			m_shooter->Spinup(4500);
			m_shooter->LiftTo(47);
			leftDist = move.Position(currentAutoTime - caseStartTime);
			rightDist = leftDist;
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_tank->Zero();
				m_autoState++; // move to next state
				//caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
			}
			break;
		case 4:{//second 90deg turn to line up towards goal
			static float pos = 0;
			m_shooter->Spinup(4500);
			m_shooter->LiftTo(47);
			switch(position){
				case 0 ... 1://positions 1 and 2 (turn counter-clockwise)
					pos += 0.20*(m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(pos, -pos);
					if(m_rateSensor->GetAngle() < 0) {
						m_tank->Zero();
						m_autoState++;
					}
					break;
				case 2 ... 3://positions 3 and 4 (turn clockwise)
					pos += 0.20*fabs(m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(-pos, pos);
					if(m_rateSensor->GetAngle() > 0) {
						m_tank->Zero();
						m_autoState++;
//						startAngle = m_rateSensor->GetAngle();
//						pixelError = m_goalVision->GetAngleCorrection();
						pos = 0;
					}
					break;
			}
			break;
		}
//		case 5: //adjust angle using vision
//			correctionAngle = pixelError*.18;
//			pos += 0.2*(correctionAngle - (m_rateSensor->GetAngle() - startAngle))/25;
//			m_tank->PositionDrive(-pos, pos);
//
//			if(fabs(correctionAngle - (m_rateSensor->GetAngle() - startAngle)) < 0.5) {
//				caseStartTime = currentAutoTime;
//				m_autoState++;
//				m_tank->Zero();
//				m_shooter->Shoot(true);
//			}
//			break;
		case 6:
			m_tank->Zero();
			m_shooter->LiftTo(47);
			m_shooter->Spinup(0);
			break;
	}


	if (count%10 == 0){
		SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
		SmartDashboard::PutNumber("Auto Left distance", leftDist);
		SmartDashboard::PutNumber("Auto Right distance", rightDist);
	}
	count ++; // increment counter
	lastState = m_autoState;

}

void Autonomous::Portcullis(){
	static unsigned count = 0; // General Purpose counter
	static double caseStartTime = 0;
	float leftDist = 0,rightDist = 0;
	static unsigned lastState = 99; // used to detect state change
	static float startAngle = 0;
	static float timeAdjust = 0;
	static TrapezoidalMove move;
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;

	if(m_init) { // Initilize static variables.
		lastState = 0;
		count = 0;
		caseStartTime = currentAutoTime;
		move.SetAll(12,12,15,62);
		startAngle = m_rateSensor->GetAngle();
		timeAdjust = 0;
		m_tank->StraightDrive(0, 0, false);
		m_suspension->SetFrontLeft(true);
		m_suspension->SetBackLeft(false);
		m_suspension->SetFrontRight(true);
		m_suspension->SetBackRight(false);
		m_init = false;
	}

	printf("case: %i   auto time: %f    right:%f  left:%f\n",m_autoState,currentAutoTime,rightDist,leftDist);
	switch (m_autoState){
		case 0: //reach portcullis
			leftDist = -move.Position(currentAutoTime);
			rightDist = -move.Position(currentAutoTime);
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist < -53.5) {
				m_shooter->LiftTo(175);
				m_suspension->SetFrontLeft(false);
				m_suspension->SetFrontRight(false);
			}
			else m_shooter->LiftTo(170);
			if (currentAutoTime > move.GetTotalTime()) {
				caseStartTime = currentAutoTime;
				move.SetAll(12,12,15,100);
				m_tank->Zero();
				m_autoState++;

			}
			break;
		case 1://start lifting
			m_shooter->LiftTo(91);
			leftDist = -move.Position(currentAutoTime - caseStartTime);
			rightDist = -move.Position(currentAutoTime - caseStartTime);
			m_tank->PositionDrive(leftDist, rightDist);
			if (m_shooter->GetLiftAngle() < 90) m_autoState++;
			break;
		case 2://
			timeAdjust += 0.05;
			currentAutoTime += timeAdjust;
			leftDist = -move.Position(currentAutoTime - caseStartTime);
			rightDist = -move.Position(currentAutoTime - caseStartTime);
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_tank->Zero();
				}
			break;
		case 3://be done
			m_tank->Zero();
			break;
	}

	if (count%10 == 0){
		SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
		SmartDashboard::PutNumber("Auto Left distance", leftDist);
		SmartDashboard::PutNumber("Auto Right distance", rightDist);
	}
	count ++; // increment counter
	lastState = m_autoState;
}

void Autonomous::RockWall(int position){ //This now uses vision
	static unsigned count = 0; // General Purpose counter
	static float adjust = 0;
	static double caseStartTime = 0;
	float leftDist,rightDist;
	static unsigned lastState = 99; // used to detect state change
	static float startAngle = 0;
	static float timeAdjust = 0;
	static TrapezoidalMove move;
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;
	static int finalMoveDist = 0;
	static float correctionAngle = 0;
	static float pixelError = 0;

	if(m_init) { // Initilize static variables.
		switch(position) {//set distance of final move based on starting position
		case 0: finalMoveDist = 84; break;
		case 1: finalMoveDist = 31; break;
		case 2: finalMoveDist = 22; break;
		case 3: finalMoveDist = 75; break;
		}
		lastState = 0;
		count = 0;
		caseStartTime = currentAutoTime;
		move.SetAll(24,24,36,218);
		startAngle = m_rateSensor->GetAngle();
		timeAdjust = 0;
		adjust = 0;
		m_tank->StraightDrive(0, 0, false);
		m_suspension->SetFrontLeft(true);
		m_suspension->SetBackLeft(true);
		m_suspension->SetFrontRight(true);
		m_suspension->SetBackRight(true);
		m_init = false;
	}
	leftDist = move.Position(currentAutoTime) + adjust/2;
	rightDist = move.Position(currentAutoTime) - adjust/2;

	switch (m_autoState){
		case 0:  // Drive over rough terrain with wheels extended
			if (m_autoState != lastState){ // put things here that only need done once when entering the state

				m_shooter->Spinup(0);
				m_shooter->LiftTo(0);
			}
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist > 125) m_autoState++;
			break;
		case 1://finish the rest of the move to the tower with wheels retracted and gyro feedback
			timeAdjust += 0.01;
			currentAutoTime += timeAdjust;
			adjust += m_rateSensor->GetAngle()*0.367*0.05; //this is the tangent of 1deg times wheel separation of 21in
			m_suspension->SetFrontLeft(false);
			m_suspension->SetBackLeft(false);
			m_suspension->SetFrontRight(false);
			m_suspension->SetBackRight(false);
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_tank->Zero();
				startAngle = m_rateSensor->GetAngle();
				}
			break;
		case 2://first 90deg turn to line up horizontally to tower
			static float pos = 0;
			m_shooter->Spinup(-3000);
			switch (position){
				case 0 ... 1://positions 1 and 2 (turn clockwise)
					pos += 0.2*(90 - m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(-pos, pos);
					if(m_rateSensor->GetAngle() - startAngle > 90) {
						m_autoState++; // move to next state
						caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
						move.SetAll(36,36,48,finalMoveDist); // Setup move for next step
						m_tank->Zero();
					}
					break;
				case 2 ... 3://positions 3 and 4 (turn counter-clockwise)
					pos += 0.2*fabs(90 + m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(pos, -pos);
					if(fabs(m_rateSensor->GetAngle()) + startAngle > 90) {
						m_autoState++; // move to next state
						caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
						move.SetAll(36,36,48,finalMoveDist); // Setup move for next step
						m_tank->Zero();
					}
					break;
			}
			break;
		case 3:
			m_shooter->Spinup(4500);
			m_shooter->LiftTo(47);
			leftDist = move.Position(currentAutoTime - caseStartTime);
			rightDist = leftDist;
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_tank->Zero();
				m_autoState++; // move to next state
				//caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
			}
			break;
		case 4:{//second 90deg turn to line up towards goal
			static float pos = 0;
			m_shooter->Spinup(4500);
			m_shooter->LiftTo(47);
			switch(position){
				case 0 ... 1://positions 1 and 2 (turn counter-clockwise)
					pos += 0.20*(m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(pos, -pos);
					if(m_rateSensor->GetAngle() < 0) {
						m_tank->Zero();
						m_autoState++;
//						startAngle = m_rateSensor->GetAngle();
//						pixelError = m_goalVision->GetAngleCorrection();
						pos = 0;
					}
					break;
				case 2 ... 3://positions 3 and 4 (turn clockwise)
					pos += 0.20*fabs(m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(-pos, pos);
					if(m_rateSensor->GetAngle() > 0) {
						m_tank->Zero();
						m_autoState++;
//						startAngle = m_rateSensor->GetAngle();
//						pixelError = m_goalVision->GetAngleCorrection();
						pos = 0;
					}
					break;
			}
			break;
		}
		case 5: //adjust angle using vision
			correctionAngle = pixelError*.18;
			pos += 0.2*(correctionAngle - (m_rateSensor->GetAngle() - startAngle))/25;
			m_tank->PositionDrive(-pos, pos);

			if(fabs(correctionAngle - (m_rateSensor->GetAngle() - startAngle)) < 0.5) {
				caseStartTime = currentAutoTime;
				m_autoState++;
				m_tank->Zero();
				m_shooter->Shoot(true);
			}
			break;
		case 6:
			m_tank->Zero();
			m_shooter->LiftTo(47);
			m_shooter->Spinup(0);
			break;
	}


	if (count%10 == 0){
		SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
		SmartDashboard::PutNumber("Auto Left distance", leftDist);
		SmartDashboard::PutNumber("Auto Right distance", rightDist);
	}
	count ++; // increment counter
	lastState = m_autoState;

}

void Autonomous::Moat(int position){ //This now uses vision
	static unsigned count = 0; // General Purpose counter
	static float adjust = 0;
	static double caseStartTime = 0;
	float leftDist,rightDist;
	static unsigned lastState = 99; // used to detect state change
	static float startAngle = 0;
	static float timeAdjust = 0;
	static TrapezoidalMove move;
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;
	static int finalMoveDist = 0;
	static float correctionAngle = 0;
	static float pixelError = 0;

	if(m_init) { // Initilize static variables.
		switch(position) {//set distance of final move based on starting position
		case 0: finalMoveDist = 84; break;
		case 1: finalMoveDist = 31; break;
		case 2: finalMoveDist = 22; break;
		case 3: finalMoveDist = 75; break;
		}
		lastState = 0;
		count = 0;
		caseStartTime = currentAutoTime;
		move.SetAll(24,24,36,225);
		startAngle = m_rateSensor->GetAngle();
		timeAdjust = 0;
		adjust = 0;
		m_tank->StraightDrive(0, 0, false);
		m_suspension->SetFrontLeft(true);
		m_suspension->SetBackLeft(true);
		m_suspension->SetFrontRight(true);
		m_suspension->SetBackRight(true);
		m_init = false;
	}
	leftDist = move.Position(currentAutoTime) + adjust/2;
	rightDist = move.Position(currentAutoTime) - adjust/2;

	switch (m_autoState){
		case 0:  // Drive over rough terrain with wheels extended
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if (rightDist > 135) m_autoState++;
			break;
		case 1://finish the rest of the move to the tower with wheels retracted and gyro feedback
			timeAdjust += 0.01;
			currentAutoTime += timeAdjust;
			adjust += m_rateSensor->GetAngle()*0.367*0.05; //this is the tangent of 1deg times wheel separation of 21in
			m_suspension->SetFrontLeft(false);
			m_suspension->SetBackLeft(false);
			m_suspension->SetFrontRight(false);
			m_suspension->SetBackRight(false);
			leftDist = move.Position(currentAutoTime) + adjust/2;
			rightDist = move.Position(currentAutoTime) - adjust/2;
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_tank->Zero();
				startAngle = m_rateSensor->GetAngle();
				}
			break;
		case 2://first 90deg turn to line up horizontally to tower
			static float pos = 0;
			m_shooter->Spinup(-3000);
			switch (position){
				case 0 ... 1://positions 1 and 2 (turn clockwise)
					pos += 0.2*(90 - m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(-pos, pos);
					if(m_rateSensor->GetAngle() - startAngle > 90) {
						m_autoState++; // move to next state
						caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
						move.SetAll(36,36,48,finalMoveDist); // Setup move for next step
						m_tank->Zero();
					}
					break;
				case 2 ... 3://positions 3 and 4 (turn counter-clockwise)
					pos += 0.2*fabs(90 + m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(pos, -pos);
					if(fabs(m_rateSensor->GetAngle()) + startAngle > 90) {
						m_autoState++; // move to next state
						caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
						move.SetAll(36,36,48,finalMoveDist); // Setup move for next step
						m_tank->Zero();
					}
					break;
			}
			break;
		case 3:
			m_shooter->Spinup(4500);
			m_shooter->LiftTo(45);
			leftDist = move.Position(currentAutoTime - caseStartTime);
			rightDist = leftDist;
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_tank->Zero();
				m_autoState++; // move to next state
				//caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
			}
			break;
		case 4:{//second 90deg turn to line up towards goal
			static float pos = 0;
			m_shooter->Spinup(4500);
			m_shooter->LiftTo(45);
			switch(position){
				case 0 ... 1://positions 1 and 2 (turn counter-clockwise)
					pos += 0.20*(m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(pos, -pos);
					if(m_rateSensor->GetAngle() < 0) {
						m_tank->Zero();
						m_autoState++;
//						startAngle = m_rateSensor->GetAngle();
//						pixelError = m_goalVision->GetAngleCorrection();
						pos = 0;
					}
					break;
				case 2 ... 3://positions 3 and 4 (turn clockwise)
					pos += 0.20*fabs(m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(-pos, pos);
					if(m_rateSensor->GetAngle() > 0) {
						m_tank->Zero();
						m_autoState++;
//						startAngle = m_rateSensor->GetAngle();
//						pixelError = m_goalVision->GetAngleCorrection();
						pos = 0;
					}
					break;
			}
			break;
		}
		case 5: //adjust angle using vision
			correctionAngle = pixelError*.18;
			pos += 0.2*(correctionAngle - (m_rateSensor->GetAngle() - startAngle))/25;
			m_tank->PositionDrive(-pos, pos);

			if(fabs(correctionAngle - (m_rateSensor->GetAngle() - startAngle)) < 0.5) {
				caseStartTime = currentAutoTime;
				m_autoState++;
				m_tank->Zero();
				m_shooter->Shoot(true);
			}
			break;
		case 6:
			m_tank->Zero();
			m_shooter->LiftTo(45);
			m_shooter->Spinup(0);
			break;
	}


	if (count%10 == 0){
		SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
		SmartDashboard::PutNumber("Auto Left distance", leftDist);
		SmartDashboard::PutNumber("Auto Right distance", rightDist);
	}
	count ++; // increment counter
	lastState = m_autoState;

}
void Autonomous::ChivalDeFrise(int position) {
	static unsigned count = 0; // General Purpose counter
	static double caseStartTime = 0;
	float leftDist = 0,rightDist = 0;
	static unsigned lastState = 99; // used to detect state change
	static float startAngle = 0;
	static TrapezoidalMove move;
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;

	if(m_init) { // Initilize static variables.
		lastState = 0;
		count = 0;
		caseStartTime = currentAutoTime;
		move.SetAll(24, 36, 36, 52);
		startAngle = m_rateSensor->GetAngle();
		//m_tank->StraightDrive(0, 0, false);
		m_init = false;
	}

	printf("case: %i   auto time: %f    right:%f  left:%f\n",m_autoState,currentAutoTime,rightDist,leftDist);
	switch (m_autoState){
		case 0: //reach chival
			if (m_autoState != lastState){ // put things here that only need done once when entering the state
				m_shooter->Spinup(0);
				m_shooter->LiftTo(135);
				m_suspension->SetFrontLeft(false);
				m_suspension->SetBackLeft(false);
				m_suspension->SetFrontRight(false);
				m_suspension->SetBackRight(false);
			}
			rightDist = -move.Position(currentAutoTime);
			leftDist = rightDist;
			//m_tank->StraightDrive(-rightDist, startAngle - m_rateSensor->GetAngle());
			m_tank->Drive(rightDist, leftDist);
			if (currentAutoTime - caseStartTime  > move.GetTotalTime()){
				m_autoState++;
				m_tank->Zero();
			}
			break;
		case 1://start lowering
			m_shooter->LiftTo(180);
			printf("lift angle: %f\n", m_shooter->GetLiftAngle());
			if (m_shooter->GetLiftAngle() > 170) {
				m_autoState++;
				move.SetAll(12, 12, 18, 92);
				caseStartTime = currentAutoTime;
			}
			break;
		case 2:
			leftDist = -move.Position(currentAutoTime-caseStartTime);
			rightDist = -move.Position(currentAutoTime-caseStartTime);
			m_tank->PositionDrive(leftDist, rightDist);
			if(rightDist < -5) {
				m_shooter->LiftTo(0);
			}
			if(rightDist < -30) {
				m_suspension->SetFront(true);
			}
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_tank->Zero();
			}
			break;
		case 3://first 90deg turn to line up horizontally to tower
			static float pos = 0;
			m_suspension->SetFront(false);
			m_shooter->Spinup(4700);
			switch (position){
				case 0://position 2(turn clockwise)
					m_shooter->LiftTo(130);
					pos += 0.2*(31 - m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(-pos, pos);
					if(m_rateSensor->GetAngle() - startAngle > 31) {
						m_autoState++; // move to next state
						m_tank->Zero();
						caseStartTime = currentAutoTime;
					}
					break;
				case 1://position 3(turn clockwise)
					m_shooter->LiftTo(129);
					pos += 0.2*(13.5 - m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(-pos, pos);
					if(m_rateSensor->GetAngle() - startAngle > 13.5) {
						m_autoState++; // move to next state
						m_tank->Zero();
						caseStartTime = currentAutoTime;
					}
					break;
				case 2://position 4(turn counter-clockwise)
					m_shooter->LiftTo(129);
					pos += 0.2*fabs(9.5 + m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(pos, -pos);
					if(m_rateSensor->GetAngle() - startAngle < -9.5) {
						m_autoState++; // move to next state
						m_tank->Zero();
						caseStartTime = currentAutoTime;
					}
					break;
				case 3://position 4(turn clockwise)
					m_shooter->LiftTo(129);
					pos += 0.2*(22 - m_rateSensor->GetAngle())/25 + 0.01;
					m_tank->PositionDrive(pos, -pos);
					if(m_rateSensor->GetAngle() - startAngle < -22) {
						m_autoState++; // move to next state
						m_tank->Zero();
						caseStartTime = currentAutoTime;
					}
				break;
			}
			break;
			case 4:
				if (currentAutoTime - caseStartTime > 1.25) m_shooter->Shoot(true);
				break;
	}

	if (count%10 == 0){
		SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
		SmartDashboard::PutNumber("Auto Left distance", leftDist);
		SmartDashboard::PutNumber("Auto Right distance", rightDist);
	}
	count ++; // increment counter
	lastState = m_autoState;
}
void Autonomous::TwoBallLowBar() {
	static unsigned count = 0; // General Purpose counter
	static float startAngle = 0;

	static double caseStartTime = 0;
	float leftDist,rightDist;
	static unsigned lastState = 99; // used to detect state change

	static TrapezoidalMove move;
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;

	if(m_init) { // Initilize static variables.
		lastState = 0;
		count = 0;
		caseStartTime = currentAutoTime;
		move.SetAll(24,36,72,155);
		startAngle = m_rateSensor->GetAngle();
		m_tank->StraightDrive(0, 0, false);
		m_init = false;
	}

	switch (m_autoState){
		case 0:  // Drive to low bar
			if (m_autoState != lastState){ // put things here that only need done once when entering the state
				caseStartTime = currentAutoTime;
				move.SetAll(24,36,72,155);
				move.CalcParams();
				startAngle = m_rateSensor->GetAngle();
				m_tank->StraightDrive(0, 0, false);
			}

			leftDist = move.Position(currentAutoTime);
			rightDist = leftDist;
			m_tank->StraightDrive(-move.Position(currentAutoTime),startAngle - m_rateSensor->GetAngle());
			m_shooter->Spinup(-2000);
			if (rightDist > 135) m_shooter->LiftTo(35);
			else m_shooter->LiftTo(165);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_tank->Zero();
			}

			break;
		case 1: // Turn 60 degrees - wheels equal and opposite directions 1/4 of circle circumference
			static float pos = 0;
			pos += 0.2;
			m_tank->PositionDrive(-pos, pos);
			m_shooter->LiftTo(35);
			m_shooter->Spinup(4700);
			if(m_rateSensor->GetAngle() - startAngle > 37) {
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_tank->Zero();
			}
			break;
		case 2: // wait for spinup
			if (fabs(fabs(m_shooter->GetWheelRPM()) - 4700) < 25){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
			}
			break;
		case 3: // shoot
			m_shooter->Shoot(true);
			if ((currentAutoTime - caseStartTime) > 0.10){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_shooter->Spinup(0);
				pos = 0;
			}
			break;
		case 4: // Turn 60 degrees - wheels equal and opposite directions 1/4 of circle circumference
			pos += 0.2;
			m_tank->PositionDrive(pos, -pos);
			m_shooter->LiftTo(170);
			if(m_rateSensor->GetAngle() < 0) {
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				move.SetAll(24,36,72,140); // Setup move for next step
				m_tank->Zero();

			}
			break;
		case 5:
			leftDist = -move.Position(currentAutoTime - caseStartTime);
			rightDist = leftDist;
			m_tank->StraightDrive(leftDist,startAngle - m_rateSensor->GetAngle());
			break;
	}


	if (count%10 == 0){
		SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
		SmartDashboard::PutNumber("Auto Left distance", leftDist);
		SmartDashboard::PutNumber("Auto Right distance", rightDist);
	}
	count ++; // increment counter
	lastState = m_autoState;

}
