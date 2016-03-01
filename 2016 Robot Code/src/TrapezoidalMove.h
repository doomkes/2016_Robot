/*
 * TrapezoidalMove.h
 *
 *  Created on: Feb 29, 2016
 *      Author: Win7
 */

#ifndef TRAPEZOIDALMOVE_H_
#define TRAPEZOIDALMOVE_H_

class TrapezoidalMove {
	float m_accel, m_decel, m_max_speed, m_distance, m_initialPos, m_targetPos;
	float m_t1, m_t2, m_t3;
public:
	TrapezoidalMove();
	TrapezoidalMove(float Accel, float Decel, float MaxSpeed, float Distance);
	TrapezoidalMove(float Accel, float Decel, float MaxSpeed, float InitialPos, float TargetPos);
	void SetInitialPos(float);
	void SetTargetPos(float);
	void SetAccel(float);		// feet per second per second
	void SetDecel(float);		// feet per second per second
	void SetMaxSpeed(float);	// feet per second
	void SetDistance(float);
	void CalcParams(void);
	float Position(float time);
	float GetTotalTime();
};

#endif /* TRAPEZOIDALMOVE_H_ */
