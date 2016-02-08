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
public:
	Shooter();
	virtual ~Shooter();

	void Rotate(float speed);
	void Shoot();
	void Pickup();
};

#endif /* SRC_SHOOTER_H_ */
