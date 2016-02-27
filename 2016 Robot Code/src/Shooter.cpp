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
		m_kicker(SHOOTERSOL),
		m_stopAt(1e100)
{
	m_shoot1.SetControlMode(CANSpeedController::kVoltage);
	m_shoot2.SetControlMode(CANSpeedController::kVoltage);

	m_lift.SetControlMode(CANSpeedController::kPosition);
	m_lift.SetFeedbackDevice(CANTalon::CtreMagEncoder_Relative);
	m_lift.SetPID(1.0, 0.01, 0);

	m_runtime.Start();
}


Shooter::~Shooter()
{
}

void Shooter::Shoot(bool val)
{
	m_kicker.Set(val);
	m_stopAt = m_runtime.Get() + 1.0;
}

void Shooter::Spinup(float speed) {
	m_shoot1.Set(speed);
	m_shoot2.Set(-speed);
}

void Shooter::Pickup()
{
	m_shoot1.Set(-9);
	m_shoot2.Set(9);

	//m_stopAt = m_runtime.Get() + 1.0;
}

void Shooter::Lift(float increment) {
	m_lift.Set(increment*6);
}

void Shooter::LiftTo(float position) {
	m_lift.Set(position);
}

void Shooter::Update() {
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
