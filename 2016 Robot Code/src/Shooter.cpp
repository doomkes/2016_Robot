/*
 * Shooter.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#include <Shooter.h>
#include "robotmap.h"

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
	m_lift.SetPID(1, 0.0, 0.0);
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
	float position = angle;// * 0.00277778; //multiplying shooter angle by this number gives a value from 0 to 0.5 (range of shooter)
	delta_time = Timer::GetFPGATimestamp() - last_time;
	if (current_position < position)
	{
		current_position = current_position + max_speed * delta_time;
		if (current_position > position)
		{
			current_position = position;
		}
	}
	else if (current_position > position)
	{
		current_position = current_position - max_speed * delta_time;
		if (current_position < position)
		{
			current_position = position;
		}
	}
	m_lift.Set(current_position);
	last_time = Timer::GetFPGATimestamp();

	target_position = position;
}

void Shooter::Update() {
	m_lift.SetPID(SmartDashboard::GetNumber("a", 1), 0.0, 0.0);
	//if(m_runtime.Get() >= m_stopAt) {
	//	m_shoot1.Set(0);
	//	m_shoot2.Set(0);
	//	m_kicker.Set(false);
	//}
}

void Shooter::Stop() {
	m_shoot1.Set(0);
	m_shoot2.Set(0);
}
void Shooter::ShooterLiftZero()
{
	switch (shooter_zero)	{//automatic zero encoder
			case 0:
				break;
			case 1:	//bring shooter down until it hits the limit switch
				LiftTo(100);
				if (m_lift.IsFwdLimitSwitchClosed()) shooter_zero = 2;
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
	return target_position;// / 0.00277778;
}
