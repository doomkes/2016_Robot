/*
 * Shooter.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#include <Shooter.h>
#include "robotmap.h"

Shooter::Shooter() :
		m_susan(SUSAN),
		m_lift(LIFT),
		m_shoot1(SHOOT1),
		m_shoot2(SHOOT2),
		m_kicker(SHOOTERSOL),
		m_stopAt(1e100)
{
	m_susan.SetControlMode(CANSpeedController::kPosition);
	m_susan.SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	m_susan.SetPID(1.0,0.0,0.0);//TODO calibrate.

	m_shoot1.SetControlMode(CANSpeedController::kVoltage);
	m_shoot2.SetControlMode(CANSpeedController::kVoltage);

	m_lift.SetControlMode(CANSpeedController::kPosition);
	m_lift.SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	m_lift.SetPID(1.0,0.0,0.0);//TODO calibrate.

	m_runtime.Start();
}

void Shooter::Rotate(float incr) {
	m_susan.Set(m_susan.Get()+incr);
}

Shooter::~Shooter()
{
}

void Shooter::RotateTo(float position)
{
	m_susan.Set(position);
}

void Shooter::Shoot()
{
	m_kicker.Set(true);
	m_stopAt = m_runtime.Get() + 1.0;
}

void Shooter::Spinup(float speed) {
	m_shoot1.Set(speed);
	m_shoot2.Set(speed);
}

void Shooter::Pickup()
{
	m_shoot1.Set(-1);
	m_shoot2.Set(-1);

	m_stopAt = m_runtime.Get() + 1.0;
}

void Shooter::Lift(float increment) {
	m_lift.Set(m_lift.Get()+increment);
}

void Shooter::LiftTo(float position) {
	m_lift.Set(position);
}

void Shooter::Update() {
	if(m_runtime.Get() >= m_stopAt) {
		m_shoot1.Set(0);
		m_shoot2.Set(0);
		m_kicker.Set(false);
	}
}
