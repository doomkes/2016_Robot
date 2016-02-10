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
	CANTalon m_susan, m_lift, m_shoot1, m_shoot2;
	Timer m_runtime; // XXX: Replace with global timer?
	double m_stopAt;
public:
	Shooter();
	virtual ~Shooter();

	void RotateTo(float position);
	void Shoot();
	void Pickup();
	/**
	 * Call every loop to update async actions.
	 */
	void Update();
};

#endif /* SRC_SHOOTER_H_ */
