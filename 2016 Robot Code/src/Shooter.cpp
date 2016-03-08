/*
 * Shooter.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#include <Shooter.h>
#include "robotmap.h"
#include <iostream>
using namespace std;
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
	float lifeState = 0;
	if(accel == 0) { //divide by zero protection.
		accel = 0.000001;
	}
	time2stop = (fabs(current_v) + accel * delta_t) / accel;
	dist2stop = 0.5*accel*time2stop*time2stop;
	if(error < 0) { // target position is in pos direction.
		if(current_v < 0) { //going wrong way.
			Velocity = current_v+delta_t*accel;
		}
		else {
			if(dist2stop >= fabs(error)) {
				Velocity = current_v - delta_t*accel;
			}
			else {
				if(current_v + delta_t * accel > max_v ) {
					Velocity = max_v;
				}
				else {
					Velocity = current_v + delta_t * accel;
				}
			}
		}
	}
	else {
		if(current_v > 0) {  //Error > 0.
			Velocity = current_v - delta_t * accel;
		}
		else {
			if(dist2stop >= fabs(error)) {
				Velocity = current_v + delta_t * accel;
			}
			else {
				if (current_v - delta_t * accel < -max_v) {
					Velocity = -max_v;
				}
				else {
					Velocity = current_v - delta_t * accel;
				}
			}
		}
	}
	SmartDashboard::PutNumber("State of shooter loop", lifeState);
	return Velocity;
}

Shooter::Shooter() :
		m_lift(LIFT),
		m_shoot1(SHOOT1),
		m_shoot2(SHOOT2),
		m_kicker(SHOOTERSOL),
		m_aimLight(AIM_LIGHT)
{
	m_shoot1.SetControlMode(CANSpeedController::kVoltage);
	m_shoot2.SetControlMode(CANSpeedController::kVoltage);

	m_aimLight.Set(false);

	m_lift.Reset();
	m_lift.SetControlMode(CANSpeedController::kPosition);
	m_lift.SetFeedbackDevice(CANTalon::CtreMagEncoder_Absolute);
	m_lift.SetSensorDirection(true);
	m_lift.Enable();
	m_lift.SetPID(1.0, 0.0, 0.0);

}

void Shooter::ToggleLight() {
	static bool state = false;
	m_aimLight.Set(!state);
	state = !state;
	SmartDashboard::PutBoolean("Light on", state);
}

Shooter::~Shooter()
{
}

void Shooter::Shoot(bool val)
{
	m_kicker.Set(val);
}

void Shooter::Spinup(float speed) {
	static float lastSpeed = 0;
	m_shoot1.Set(speed);
	m_shoot2.Set(-speed);

	if(speed == 0) {
		m_spinUpTimer.Stop();
	}

	if(lastSpeed == 0 && speed != 0) {
		m_spinUpTimer.Reset();
		m_spinUpTimer.Start();
	}

	lastSpeed = speed;
}

void Shooter::Pickup()
{
	m_shoot1.Set(-9);
	m_shoot2.Set(9);
}

void Shooter::SpinShoot() {
	Spinup(12);
	m_spinShootTimer.Reset();
	m_spinShootTimer.Start();
	//In Update(), shoot after a period of time has passed.
}

void Shooter::LiftTo(float angle) {
	if(angle > 175)
		angle = 175;
	if(angle < 0)
		angle = 0;
	float position = angle*SHOOTER_SCALE; //multiplying shooter angle by this number gives a value from 0 to 0.5 (range of shooter)
	m_liftAccel = SmartDashboard::GetNumber("lift accel", 0);
	m_liftMaxSpeed = SmartDashboard::GetNumber("lift max speed", 0);
	m_targetPosition = position;
}

void Shooter::Update() {
	static Timer timer;

	m_lift.SetPID(SmartDashboard::GetNumber("Shooter P", 4),
				  SmartDashboard::GetNumber("Shooter I", 0),
				  SmartDashboard::GetNumber("Shooter D", 0));
	float dt, error;
	static float velocity = 0, incr_position = 0;

	error = incr_position - m_targetPosition;
	dt = timer.Get();

	if(dt > 0.050)
		dt = 0.050;

	velocity = Velocity(dt, error, velocity, m_liftAccel, m_liftMaxSpeed);

	SmartDashboard::PutNumber("incr position", incr_position);

	incr_position = incr_position + velocity*dt;
	m_lift.Set(incr_position);

	timer.Reset();
	timer.Start();

	if(m_spinShootTimer.HasPeriodPassed(2.5)) {
		Shoot(true);
		Wait(0.5);
		Shoot(false);
		Spinup(0);
	}

	SmartDashboard::PutNumber("Spinup Time", m_spinUpTimer.Get());
	SmartDashboard::PutNumber("Closed loop error", m_lift.GetClosedLoopError());
	SmartDashboard::PutNumber("Shooter Angle", m_lift.Get()*360);
	SmartDashboard::PutNumber("lift velocity", velocity);
	SmartDashboard::PutNumber("position", m_lift.Get());
	SmartDashboard::PutNumber("target position", m_targetPosition);
	SmartDashboard::PutNumber("lift motor velocity", m_lift.GetSpeed());
	SmartDashboard::PutNumber("lift error", error);
	SmartDashboard::PutNumber("delta t", dt);
}


void Shooter::Stop() {
	Spinup(0);
}

void Shooter::Zero() {
	if(m_lift.IsRevLimitSwitchClosed()) {
		m_lift.SetPosition(0);
	}
	else {
		m_lift.SetPosition(0);
		m_lift.Set(0);

		float loopCount = 0;
		while(!m_lift.IsRevLimitSwitchClosed() && loopCount > -0.5) {
			m_lift.Set(loopCount);
			loopCount -= 0.01; // 18 deg / sec
			Wait(0.1);
		}
		m_lift.Set(0);
		m_lift.SetPosition(0);
	}
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
			m_currentPosition = 0;
			LiftTo(0);
			shooter_zero = 0;
			break;
	}
}
double Shooter::GetLiftPosition()
{
	return m_targetPosition;
}

double Shooter::GetLiftAngle() {
	return m_targetPosition / 0.00277778;
}
