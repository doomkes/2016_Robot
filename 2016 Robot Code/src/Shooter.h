/*
 * Shooter.h
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_
#include <wpilib.h>

class Shooter
{
private:
	CANTalon /*m_susan,8*/ m_lift, m_shoot1, m_shoot2;
	Solenoid m_kicker;
	float max_speed = 0.5;
	float delta_time = 0;
	float last_time = 0;
	float current_position = 0;
	float target_position = 0;
public:
	Shooter();
	virtual ~Shooter();

	//void RotateTo(float position);
	//void Rotate(float increment);
	void LiftTo(float angle);
	void Shoot(bool val);
	void Spinup(float speed);
	void Pickup();
	void Stop();
	/**
	 * Call every loop to update async actions.
	 */
	void Update();
	void ShooterLiftZero();
	double GetLiftPosition();
	int shooter_zero = 0;
};

#endif /* SRC_SHOOTER_H_ */
