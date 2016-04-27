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
	return Velocity;
}

Shooter::Shooter() :
		m_lift(LIFT),
		m_shoot1(SHOOT1),

		m_shoot2(SHOOT2),
		m_kicker(SHOOTERSOL),
		m_runNgunLight(RUN_GUN_LIGHT)

{
	m_shoot1.SetControlMode(CANSpeedController::kVoltage);
	m_shoot2.SetControlMode(CANSpeedController::kVoltage);

	m_shoot1.SetFeedbackDevice(CANTalon::QuadEncoder);
	m_shoot1.ConfigNominalOutputVoltage(+0.0, -0.0);
	m_shoot1.ConfigPeakOutputVoltage(+12.0, -12.0);
	m_shoot1.SetControlMode(CANSpeedController::kSpeed);
	m_shoot1.SetSensorDirection(true);
	m_shoot1.SetInverted(true);
	m_shoot1.SetIzone(200);
	m_shoot1.ConfigEncoderCodesPerRev(20);
	m_shoot1.SetPID(4,0.01,0);
	m_shoot1.SetF(1.5);
	m_shoot1.SetVoltageRampRate(50);

	m_shoot2.SetFeedbackDevice(CANTalon::QuadEncoder);
	m_shoot2.ConfigNominalOutputVoltage(+0.0, -0.0);
	m_shoot2.ConfigPeakOutputVoltage(+12.0, -12.0);
	m_shoot2.SetControlMode(CANSpeedController::kSpeed);
	m_shoot2.SetSensorDirection(true);
	m_shoot2.SetInverted(true);
	m_shoot2.SetIzone(200);
	m_shoot2.ConfigEncoderCodesPerRev(20);
	m_shoot2.SetPID(4,0.01,0);
	m_shoot2.SetF(1.5);//(16.620);
	m_shoot2.SetVoltageRampRate(50);

	m_runNgunLight.SetPWMRate(1000);
	m_runNgunLight.EnablePWM(0.05);
	m_runNgunLight.Set(0);
	m_lift.Reset();
	m_lift.SetControlMode(CANSpeedController::kPosition);
	m_lift.SetFeedbackDevice(CANTalon::CtreMagEncoder_Absolute);
	m_lift.SetSensorDirection(true);
	m_lift.Enable();
	m_lift.SetPID(1.0, 0.0, 0.0);

}

void Shooter::ToggleRunLight(bool state) {
	//static bool state = false;
	//m_runNgunLight.Set(state);
	if(state)
		m_runNgunLight.UpdateDutyCycle(0.7);
	else
		m_runNgunLight.UpdateDutyCycle(0);

	//state = !state;
	//SmartDashboard::PutBoolean("Light on", state);
}

Shooter::~Shooter()
{
}

void Shooter::Shoot(bool val)
{
	m_kicker.Set(val);
}
void Shooter::LimitShoot(bool val, double rate) {
	if((fabs(m_shoot1.GetSpeed()) >= m_targetWheelSpeed - 50
			&& m_targetWheelSpeed != 0 && fabs(rate) < 3) || m_targetWheelSpeed < 0) {
		m_kicker.Set(val);
	} else m_kicker.Set(false);
}
void Shooter::Spinup(float speed) {
//	static float lastSpeed = 0;
	m_targetWheelSpeed = speed;


	m_shoot1.Set(speed);
	m_shoot2.Set(-speed);

//	if(lastSpeed == 0 && speed != 0) {
//		m_spinUpTimer.Reset();
//		m_spinUpTimer.Start();
//	} else if(lastSpeed != 0 && speed == 0) {
//		m_spinUpTimer.Reset();
//	}

//	lastSpeed = speed;
}

void Shooter::Pickup()
{
	m_shoot1.Set(-3000);
	m_shoot2.Set(3000);
	m_targetWheelSpeed = -3000;
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
	m_liftAccel = SmartDashboard::GetNumber("lift accel", 1);
	m_liftMaxSpeed = SmartDashboard::GetNumber("lift max speed", 0.5);
	m_targetPosition = position;
	m_lift.SetPID(SmartDashboard::GetNumber("Shooter P", 4), //TODO hard code values/perfernces
				  SmartDashboard::GetNumber("Shooter I", 0.01),
				  SmartDashboard::GetNumber("Shooter D", 0));
}

void Shooter::Update() {
	static Timer timer;
	static unsigned count = 0;
	printf("error: %f\n", m_shoot1.GetSpeed());
	if(fabs(m_shoot1.GetSpeed()) >= m_targetWheelSpeed - 50 && m_targetWheelSpeed != 0) {
		if(count % 10 == 0) {
			SmartDashboard::PutBoolean("At Target Speed", true);
		}
	} else {
		if(count % 10 == 0) {
			SmartDashboard::PutBoolean("At Target Speed", false);
		}
	}
	count++;
//	cout << "Target speed: " << m_targetWheelSpeed << endl;
//
//	cout << "Shoot1 output: " << m_shoot1.GetOutputVoltage()/m_shoot1.GetBusVoltage() << endl;
//	cout << "Shoot1 speed: " << m_shoot1.GetSpeed() << endl;
//	cout << "Shoot1 Error: " << m_shoot1.GetClosedLoopError() << endl;
//
//	cout << "Shoot2 output: " << m_shoot2.GetOutputVoltage()/m_shoot2.GetBusVoltage() << endl;
//	cout << "Shoot2 speed: " << m_shoot2.GetSpeed() << endl;
//	cout << "Shoot2 Error: " << m_shoot2.GetClosedLoopError() << endl;

	float dt, error;
	static float velocity = 0;
	static float incr_position = 0;
	static unsigned loopCount = 0;
	if(m_liftZero == 0) {
		error = incr_position - m_targetPosition;
		dt = timer.Get();

		if(dt > 0.050) {
			dt = 0.050;
		}
		velocity = Velocity(dt, error, velocity, m_liftAccel, m_liftMaxSpeed);

		//SmartDashboard::PutNumber("incr position", incr_position);

		incr_position = incr_position + velocity*dt;

		timer.Reset();
		timer.Start();
	}
	else if(m_liftZero == 1) {
		if(m_lift.IsRevLimitSwitchClosed() || loopCount > 100) {
			m_lift.SetPosition(0);
			incr_position = 0;
			m_liftZero = 0;
			velocity = 0;
			loopCount = 0;
		}
		else {
			if(fabs(incr_position - m_lift.Get()) > 0.05) {
				incr_position = m_lift.Get();
			}
			incr_position -= 0.005;
			loopCount++;
		}
	}

	m_lift.Set(incr_position);

//	SmartDashboard::PutNumber("Spinup Time", m_spinUpTimer.Get());
//	SmartDashboard::PutNumber("Closed loop error", m_lift.GetClosedLoopError());
//	SmartDashboard::PutNumber("Shooter Angle", m_lift.Get()*360);
//	SmartDashboard::PutNumber("lift velocity", velocity);
//	SmartDashboard::PutNumber("position", m_lift.Get());
//	SmartDashboard::PutNumber("target position", m_targetPosition);
//	SmartDashboard::PutNumber("lift motor velocity", m_lift.GetSpeed());
//	SmartDashboard::PutNumber("lift error", error);
//	SmartDashboard::PutNumber("delta t", dt);
	}

void Shooter::Stop() {
	Spinup(0);
}

void Shooter::Zero() {
	m_liftZero = 1;

}

double Shooter::GetLiftPosition()
{
	return m_lift.GetPosition();
}

double Shooter::GetLiftAngle()
{
	return m_lift.GetPosition()/SHOOTER_SCALE;
}

double Shooter::GetWheelRPM() {
	return m_shoot1.GetSpeed();
}
