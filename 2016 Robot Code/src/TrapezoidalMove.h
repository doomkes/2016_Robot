/*
 * TrapezoidalMove.h
 *
 *  Created on: Feb 29, 2016
 *      Author: Win7
 */

#ifndef TRAPEZOIDALMOVE_H_
#define TRAPEZOIDALMOVE_H_

class TrapezoidalMove {
	float m_accel, m_decel, m_max_speed, m_distance, m_initialVel, m_initialPos, m_targetPos;
	float m_t1, m_t2, m_t3;
public:
	TrapezoidalMove();
	TrapezoidalMove(float Accel, float Decel, float MaxSpeed, float InitialVel = 0, float InitialPos = 0, float TargetPos = 0);
	void SetInitialPos(float);
	void SetTargetPos(float);
	void SetInitialVel(float);
	void SetAccel(float);
	void SetDecel(float);
	void SetMaxSpeed(float);
	void SetDistance(float);

	void CalcParams(void);
	float Position(float time);
	float Velocity(float time);
	float Acceleration(float time);
	float GetTotalTime();
};

#endif /* TRAPEZOIDALMOVE_H_ */
