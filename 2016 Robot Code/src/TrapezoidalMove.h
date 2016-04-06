/*
 * Test.h
 *
 *  Created on: Oct 10, 2014
 *      Author: Studio-shop
 */

/*
 * Description:
 */

#ifndef SRC_TRAPEZOIDALMOVE_H_
#define SRC_TRAPEZOIDALMOVE_H_

class TrapezoidalMove {
	float m_accel, m_decel, m_max_speed, m_distance;
	float m_t1, m_t2, m_t3;
public:
	TrapezoidalMove();
	TrapezoidalMove(float Accel, float Decel, float MaxSpeed, float Distance);
	void SetAccel(float);		// feet per second per second
	void SetDecel(float);		// feet per second per second
	void SetMaxSpeed(float);	// feet per second
	void SetDistance(float);
	void SetAll(float accel, float decel, float maxSpeed,float distance);
	void CalcParams(void);
	float GetTotalTime();
	float Position(float time);
};

#endif /* SRC_TRAPEZOIDALMOVE_H_ */
