/*
 * Autonomous.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: Win7
 */

#include <Autonomous.h>

Autonomous::Autonomous(TankDrive* tank, SuspensionDrive* suspension,Shooter* shooter)
	: m_tank(tank), m_suspension(suspension), m_shooter(shooter), m_one(1), m_two(2), m_three(3), m_four(4), m_five(5) {
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
	else if (!m_five.Get()) m_mode = 5;
	m_tank->Zero();
	m_autoState = 0;
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
		case 5:

			break;
	}
}

void Autonomous::Periodic() {
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;
	float leftDist, rightDist;
	float  curveRatio = 0.78;
	static unsigned count = 0;
	count++;
	if (count % 50 == 0)
		SmartDashboard::PutNumber("RS Angle", m_rateSensor.GetAngle());

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
			rightDist = m_move.Position(currentAutoTime);
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
				m_shooter->Spinup(-2000);
						}
			m_tank->PositionDrive(leftDist, rightDist);
			if (currentAutoTime > m_move.GetTotalTime() && currentAutoTime < (m_move.GetTotalTime() + 5))
				//m_shooter->Shoot(true);
				m_shooter->Spinup(8000);
			if (currentAutoTime > (m_move.GetTotalTime() + 3))
				m_shooter->Shoot(true);
			if (currentAutoTime > (m_move.GetTotalTime() + 5))
				m_shooter->Spinup(0);
			if (count%10 == 0){
				SmartDashboard::PutString("Auto Mode", "Low Bar Defense");
				SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
				SmartDashboard::PutNumber("Auto Left distance", leftDist);
				SmartDashboard::PutNumber("Auto Right distance", rightDist);
			}
			break;
		case 4:
			LowBar(0);
//			SmartDashboard::PutString("Auto Mode", "Low Bar Defense");
//			SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
//			rightDist = m_move.Position(currentAutoTime);
//			SmartDashboard::PutNumber("Auto Left distance", leftDist);
//			SmartDashboard::PutNumber("Auto Right distance", rightDist);
//			SmartDashboard::PutNumber("Auto Count", autoCount);
//			if (currentAutoTime < m_move.GetTotalTime())	autoCount++;
//			if (currentAutoTime > (m_move.GetTotalTime() + 2))	autoCount--;	//run trapizoidal move backwards?
//			if (rightDist < 146.84){	//get under low bar
//				leftDist = m_move.Position(currentAutoTime);
//				m_shooter->LiftTo(180);
//			}
//			if (rightDist >= 146.84){	//begin curve and raise shooter
//				leftDist = 146.84 + (rightDist - 146.84) *curveRatio;
//				m_shooter->LiftTo(42);
//			}
//			if ((rightDist >= 200) && (rightDist <= 220)){	//make sure the ball is sucked in
//				m_shooter->Spinup(-8);
//						}
//			m_tank->PositionDrive(leftDist, rightDist);	//spin up wheels until ball is shot
//			if ((rightDist >= 220) && currentAutoTime < (m_move.GetTotalTime() + 2))
//				m_shooter->Spinup(8000);
//			if (currentAutoTime > (m_move.GetTotalTime() + 1))	//wait for a sec to make sure we are lined up
//				m_shooter->Shoot(true);
//			if ((currentAutoTime > (m_move.GetTotalTime() + 2)) && (currentAutoTime > (m_move.GetTotalTime() + 2.1))) {	//Give the ball time to clear the robot, then lower shooter and de-spin wheels.
//				m_shooter->Spinup(0);
//				m_shooter->LiftTo(0);
//			}
//			if (count%10 == 0){
//				SmartDashboard::PutString("Auto Mode", "Low Bar Defense");
//				SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
//				SmartDashboard::PutNumber("Auto Left distance", leftDist);
//				SmartDashboard::PutNumber("Auto Right distance", rightDist);
//			}
			break;
		case 5:
			RockWall(0);
			break;
	}
}

void Autonomous::RockWall(unsigned position){
	static unsigned count = 0; // General Purpose counter
	static double delayStartTime = 0; // used to remember delay start times for when
									// we want to insert a pure time delay;
	static float startAngle = 0;

	static double caseStartTime = 0;
	float leftDist,rightDist;
	static unsigned lastState = 99; // used to detect state change

	static TrapezoidalMove move(24,24,36,142); // first move is 72 inches
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;


	switch (m_autoState){
		case 0:  // Drive to rock wall
			if (m_autoState != lastState){ // put things here that only need done once when entering the state
				m_suspension->SetFrontLeft(true);
				m_suspension->SetBackLeft(true);
				m_suspension->SetFrontRight(true);
				m_suspension->SetBackRight(true);
				caseStartTime = currentAutoTime;
				move.CalcParams();
				printf("hi");
			}
			printf("%i\n", 0);
			leftDist = move.Position(currentAutoTime);
			rightDist = move.Position(currentAutoTime);
			m_tank->PositionDrive(leftDist, rightDist);
			printf("current auto time %f\n", currentAutoTime);
			printf("case start time %f\n", caseStartTime);
			printf("move time %f\n", move.GetTotalTime());
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				move.SetAll(24,24,72,40); // Setup move for next step
				m_tank->Zero();
				m_suspension->SetFrontLeft(false);
				m_suspension->SetBackLeft(false);
				m_suspension->SetFrontRight(false);
				m_suspension->SetBackRight(false);
			}

			break;
		case 1: // push tight to rock wall in vbus mode
			//printf("%i\n", 2);
			m_tank->VBusDrive(-0.3,-0.3); // half drive - I think negative direction is correct.
			if ((currentAutoTime - caseStartTime) > 2){ //push for 5 seconds
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				move.SetAll(24,24,72,84); 		// Setup move for next step
				m_tank->VBusDrive(0,0); 		// stop pushing
				m_tank->SetMode(POSITION_MODE);
			}
			break;
		case 2: // Move forward
			//printf("%i\n", 3);
			leftDist = move.Position(currentAutoTime - caseStartTime);
			rightDist = move.Position(currentAutoTime - caseStartTime);
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()+ 1.0){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				float dist = 20.0 * 3.14159 / 4;  // rotate 90 degrees - 1/4 circle, wheel separation 20.5" I think
				move.SetAll(12,12,36,dist); // Setup move for next step
				startAngle = m_rateSensor.GetAngle();
				m_tank->Zero();
			}
			break;
		case 3: // Turn 90 degrees - wheels equal and opposite directions 1/4 of circle circumference
			//printf("%i\n", 4);
			static float pos = 0;
			//leftDist = move.Position(currentAutoTime - caseStartTime);
			//rightDist = -move.Position(currentAutoTime - caseStartTime);
			//m_tank->PositionDrive(leftDist, rightDist);
			m_tank->PositionDrive(-pos, pos);
			pos += 0.2;
			//if ((currentAutoTime - caseStartTime) > move.GetTotalTime() + 1.0){
			if(m_rateSensor.GetAngle() - startAngle > 90) {
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				move.SetAll(12,12,36,24); // Setup move for next step
				m_tank->Zero();
			}
			break;
		case 4:
			leftDist = move.Position(currentAutoTime - caseStartTime);
			rightDist = move.Position(currentAutoTime - caseStartTime);
			m_tank->PositionDrive(leftDist, rightDist);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()+ 1.0){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				float dist = 20.5 * 3.141159 / 4;  // rotate 90 degrees - 1/4 circle, wheel separation 20.5" I think
				move.SetAll(12,12,36,dist); // Setup move for next step
			m_tank->Zero();
			}
			break;
//		case 5: // Turn 90 degrees toward tower- wheels equal and opposite directions 1/4 of circle circumference
//			leftDist = -move.Position(currentAutoTime - caseStartTime);
//			rightDist = move.Position(currentAutoTime - caseStartTime);
//			m_tank->PositionDrive(leftDist, rightDist);
//			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()+ 1.0){
//				state++; // move to next state
//				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
//				m_shooter->Spinup(4500); // whatever the med shot should be
//				m_shooter.LiftTo(37);
//			}
//			break;
//		case 6: // get shooter angle and speed set
//
//			if ((currentAutoTime - caseStartTime) > 1.0){
//				state++; // move to next state
//				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
//
//			}
//			break;
//		case 7: // shoot
//
//			if ((currentAutoTime - caseStartTime) > 1.0){
//				state++; // move to next state
//				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
//			}
//			break;
//


	}


	if (count%10 == 0){
		SmartDashboard::PutNumber("Current Auto Time", currentAutoTime);
		SmartDashboard::PutNumber("Auto Left distance", leftDist);
		SmartDashboard::PutNumber("Auto Right distance", rightDist);
	}
	count ++; // increment counter
	lastState = m_autoState;

}

void Autonomous::LowBar(unsigned position){
	static unsigned count = 0; // General Purpose counter
	static double delayStartTime = 0; // used to remember delay start times for when
									// we want to insert a pure time delay;
	static float startAngle = 0;

	static double caseStartTime = 0;
	float leftDist,rightDist;
	static unsigned lastState = 99; // used to detect state change

	static TrapezoidalMove move(24,24,72,233);
	double currentAutoTime  = Timer::GetFPGATimestamp() - m_autoStartTime;

	switch (m_autoState){
		case 0:  // Drive to low bar
			if (m_autoState != lastState){ // put things here that only need done once when entering the state
				caseStartTime = currentAutoTime;
				move.CalcParams();
			}
			leftDist = move.Position(currentAutoTime);
			rightDist = leftDist;
			m_tank->PositionDrive(leftDist, rightDist);
			m_shooter->LiftTo(180);
			if ((currentAutoTime - caseStartTime) > move.GetTotalTime()){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				m_tank->Zero();
				startAngle = m_rateSensor.GetAngle();
			}

			break;
		case 1: // Turn 60 degrees - wheels equal and opposite directions 1/4 of circle circumference
			static float pos = 0;
			pos += 0.3;
			m_tank->PositionDrive(-pos, pos);
			if(m_rateSensor.GetAngle() - startAngle > 60) {
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
				move.SetAll(24,24,72,30); // Setup move for next step
				m_tank->Zero();
				m_shooter->Spinup(-2000); // set shooter angle and speed set
				m_shooter->LiftTo(42);
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
			}
			break;

		case 3: // wait for spinup
			if ((currentAutoTime - caseStartTime) > 2.0){
				m_autoState++; // move to next state
				caseStartTime = currentAutoTime; // reset caseStartTime since we are starting new case
			}
			break;
		case 4: // shoot
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
	lastState = m_autoState;

}

