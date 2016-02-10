/*
 * Shooter.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#include <Shooter.h>

Shooter::Shooter() :
		m_susan(5), m_lift(6), m_shoot1(7), m_shoot2(8), m_stopAt(1e100)
{
	m_susan.SetControlMode(CANSpeedController::kPosition);
	m_susan.SetFeedbackDevice(CANTalon::AnalogPot);
	m_susan.SetPID(1,0,0);//TODO calibrate.

	m_shoot1.SetControlMode(CANSpeedController::kVoltage);
	m_shoot2.SetControlMode(CANSpeedController::kVoltage);

	m_lift.SetControlMode(CANSpeedController::kPosition);
	m_lift.SetFeedbackDevice(CANTalon::AnalogPot);
	m_lift.SetPID(1,0,0);//TODO calibrate.

	m_runtime.Start();
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
	m_shoot1.Set(1);
	m_shoot2.Set(1);

	m_stopAt = m_runtime.Get() + 1.0;
}

void Shooter::Pickup()
{
	m_shoot1.Set(-1);
	m_shoot2.Set(-1);

	m_stopAt = m_runtime.Get() + 1.0;
}

void Shooter::Update() {
	if(m_runtime.Get() >= m_stopAt) {
		m_shoot1.Set(0);
		m_shoot2.Set(0);
	}
}
