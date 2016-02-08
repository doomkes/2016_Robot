/*
 * Shooter.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: Joey172
 */

#include <Shooter.h>

Shooter::Shooter() :
		m_susan(5), m_lift(6), m_shoot1(7), m_shoot2(8)
{

}

Shooter::~Shooter()
{
}

void Shooter::Rotate(float degrees)
{
	float currPos = 0;
	m_susan.Set(1);

	while(true)
	{
		if(currPos == degrees)
		{
			break;
		}
	}
	m_susan.StopMotor();
}

void Shooter::Shoot()
{

}

void Shooter::Pickup()
{

}
