/*
 * Shooter.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#include <Shooter.h>
#include "robotmap.h"
/*
 *  On each iteration, you feed this module
 *  1) the time change since the last update
 *  2) the difference between your current position and where you want to go (error)
 *  3) your last computed velocity
 *  4) your desired acceleration (used for both acclearting and decelerating)
 *  5) your max velocity
 *  this module handles positive and negative errors and velocities
 *  so if you have a negative velocity, you actually slow down by "increasing"
 *  to your velocity.  So proper signs are critical in the code below.
 *  note all of the sign flips between the "target position is in pos direction" and
 *  the "target position is in neg direction"
 */
float Velocity(float delta_t, float error, float current_v, float accel, float max_v) {
	float time2stop = 0;
	float dist2stop = 0;
	float Velocity = 0;
	if(accel == 0) { //divide by zero protection.
		return 0.0000001;
	}
	time2stop = (fabs(current_v) + accel * delta_t) / accel;
	dist2stop = 0.5*accel*time2stop*time2stop;
	if(error < 0) { // target position is in pos direction.
		if(current_v < 0) { //going wrong way.
			Velocity = current_v+delta_t*accel;
		} else if(dist2stop >= fabs(error)) {
			Velocity = current_v - delta_t*accel;
		} else if(current_v + delta_t * accel > max_v ) {
			Velocity = max_v;
		} else {
			Velocity = current_v + delta_t * accel;
		}
	} else if(current_v > 0) {  //Error > 0.
		Velocity = current_v - delta_t * accel;
	} else if(dist2stop >= fabs(error)) {
		Velocity = current_v + delta_t * accel;
	} else if (current_v - delta_t * accel < -max_v) {
		Velocity = -max_v;
	} else {
		Velocity = current_v - delta_t * accel;
	}
	return Velocity;
}

Shooter::Shooter() :
		m_lift(LIFT),
		m_shoot1(SHOOT1),
		m_shoot2(SHOOT2),
		m_kicker(SHOOTERSOL)
{
	m_shoot1.SetControlMode(CANSpeedController::kVoltage);
	m_shoot2.SetControlMode(CANSpeedController::kVoltage);

	m_lift.Reset();
	m_lift.SetControlMode(CANSpeedController::kPosition);
	m_lift.SetFeedbackDevice(CANTalon::CtreMagEncoder_Absolute);
	m_lift.SetSensorDirection(true);
	m_lift.Enable();
	m_lift.SetPID(1.0, 0.0, 0.0);

}


Shooter::~Shooter()
{
}

void Shooter::Shoot(bool val)
{
	m_kicker.Set(val);
}

void Shooter::Spinup(float speed) {
	m_shoot1.Set(speed);
	m_shoot2.Set(-speed);
}

void Shooter::Pickup()
{
	m_shoot1.Set(-9);
	m_shoot2.Set(9);
}

void Shooter::LiftTo(float angle) {
	float position = angle; // * 0.00277778; //multiplying shooter angle by this number gives a value from 0 to 0.5 (range of shooter)
	//delta_time = Timer::GetFPGATimestamp() - last_time;
	m_liftMove.SetAccel(SmartDashboard::GetNumber("lift accel", 1));
	m_liftMove.SetDecel(SmartDashboard::GetNumber("lift decel", 1));
	m_liftMove.SetMaxSpeed(SmartDashboard::GetNumber("lift max speed", 1));
	m_liftMove.SetDistance(-position);
	//m_liftMove.SetInitialPos(0);
	//m_liftMove.SetInitialVel(0);
	//m_liftMove.SetTargetPos(position);
	m_liftMove.CalcParams();
	m_timer.Reset();
	m_timer.Start();
//	if (current_position < position)
//	{
//		current_position = current_position + max_speed * delta_time;
//		if (current_position > position)
//		{
//			current_position = position;
//		}
//	}
//	else if (current_position > position)
//	{
//		current_position = current_position - max_speed * delta_time;
//		if (current_position < position)
//		{
//			current_position = position;
//		}
//	}
//	m_lift.Set(current_position);
	//last_time = Timer::GetFPGATimestamp();

	target_position = position;
}

void Shooter::Update() {
	static Timer timer;

	m_lift.SetPID(SmartDashboard::GetNumber("Shooter P", 0.0) ,
				  SmartDashboard::GetNumber("Shooter I", 0.0),
				  SmartDashboard::GetNumber("Shooter D", 0.0));
	SmartDashboard::PutNumber("position", m_lift.Get());
	SmartDashboard::PutNumber("target position", target_position);
	//m_lift.Set(m_liftMove.Position(m_timer.Get()));
	m_lift.Set(m_lift.Get() +
			Velocity(timer.Get(),
					m_lift.Get() - target_position,
					m_lift.GetSpeed(),
					SmartDashboard::GetNumber("lift accel", 0),
					SmartDashboard::GetNumber("lift max speed", 0)));

	timer.Reset();
	timer.Start();
}


void Shooter::Stop() {
	m_shoot1.Set(0);
	m_shoot2.Set(0);
}

void Shooter::ShooterLiftZero()
{
	switch (shooter_zero) {//automatic zero encoder
		case 0:
			break;
		case 1:	//bring shooter down until it hits the limit switch
			LiftTo(100);
			if (m_lift.IsFwdLimitSwitchClosed())
				shooter_zero = 2;
			break;
		case 2:	//stop it,zero encoder, and bring to position 1
			m_lift.SetPosition(0);
			current_position = 0;
			LiftTo(0);
			shooter_zero = 0;
			break;
	}
}
double Shooter::GetLiftPosition()
{
	return target_position;
}

double Shooter::GetLiftAngle() {
	return target_position / 0.00277778;
}
