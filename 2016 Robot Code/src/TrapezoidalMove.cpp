/*
* TrapezoidalMove.cpp
*
*  Created on: Feb 29, 2016
*      Author: Win7
*/

#include "TrapezoidalMove.h"
#include <math.h>
using namespace std;

TrapezoidalMove::TrapezoidalMove() {
	m_accel = 0;
	m_decel = 0;
	m_max_speed = 0;
	m_distance= 0;
	m_initialVel = 0;
	m_initialPos = 0;
	m_targetPos = 0;
	m_t1 = 0;
	m_t2 = 0;
	m_t3 = 0;
}

TrapezoidalMove::TrapezoidalMove(float Accel, float Decel, float MaxSpeed, float InitialVel, float InitialPos, float TargetPos)
{
	m_t1 = -99;
	m_t2 = -99;
	m_t3 = -99;
	m_accel = Accel;
	m_decel = Decel;
	m_max_speed = MaxSpeed;
	m_distance = fabs(InitialPos - TargetPos);
	m_initialVel = InitialVel;
	m_initialPos = InitialPos;
	m_targetPos = TargetPos;
	this->CalcParams();
}

void TrapezoidalMove::SetTargetPos(float targetPos) {
	m_targetPos = targetPos;
}

void TrapezoidalMove::SetInitialPos(float initialPos) {
	m_initialPos = initialPos;
}

void TrapezoidalMove::SetAccel (float value) {
	m_accel = value;
}

void TrapezoidalMove::SetDecel(float value) {
	m_decel = value;
}

void TrapezoidalMove::SetMaxSpeed(float value) {
	m_max_speed = value;
}

void TrapezoidalMove::SetDistance(float value) {
	m_distance = value;
}

void TrapezoidalMove::CalcParams(void) {
	float accel_time, decel_time, min_dist, accel_dist, decel_dist;

	accel_time = m_max_speed / m_accel;
	decel_time = m_max_speed / m_decel;
	accel_dist = 0.5*m_accel * accel_time*accel_time;
	decel_dist = 0.5*m_decel * decel_time*decel_time;
	min_dist = accel_dist + decel_dist;
	if (min_dist > m_distance) // never gets up to speed.
	{
		m_t1 = pow(2.0* m_distance / (m_accel + (m_accel*m_accel) / m_decel), 0.5);
		m_t2 = m_t1; // no constant velocity time
		m_t3 = m_accel / m_decel*m_t1 + m_t1;
	} else {
		m_t1 = accel_time;
		m_t2 = (m_distance - min_dist) / m_max_speed + m_t1; // time at end of constanct velocity
		m_t3 = decel_time + m_t2;
	};
}

float TrapezoidalMove::GetTotalTime() {
	return m_t3;
}

float TrapezoidalMove::Position(float time) {
	float position = 0.0;
	if (time < m_t1)
		position = 0.5*m_accel*time*time;
	else if (time < m_t2)
		position = 0.5*m_accel*m_t1*m_t1 + (time - m_t1)*m_max_speed;
	else if (time <= m_t3)
		position = 0.5*m_accel*m_t1*m_t1
		+ (m_t2 - m_t1)*m_max_speed
		+ 0.5*m_decel*(m_t3 - m_t2)*(m_t3 - m_t2) - 0.5*m_decel*(m_t3 - time)*(m_t3 - time);
	else position = m_distance;

	return m_initialPos + position;
}

float TrapezoidalMove::Velocity(float time)
{
	//TODO: Implement.
	return 0;
}

float TrapezoidalMove::Acceleration(float time)
{
	//TODO: Implement.
	return 0;
}
