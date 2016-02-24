/*
 * UserInterface.h
 *
 *  Created on: Jan 20, 2016
 *      Author: James
 */

#ifndef SRC_USERINTERFACE_H_
#define SRC_USERINTERFACE_H_

#include "WPILib.h"

struct WedgemoreUserInput
{
	bool DropFR = false, DropFL = false, DropBR = false, DropBL = false;
	float LeftSpeed = 0.0, RightSpeed = 0.0;
	float RotateSpeed = 0;
	float LiftSpeed = 0;
	bool Shoot = false, Pickup = false;
	bool SpinUp = false;
	bool LowGoal = false, HiGoal = false, AutoTrack = false, PickupPos = false, StartPos = false;

};

class UserInterface
{
private:
	Joystick m_lStick, m_rStick, m_manStick;
	void GetManStickValues(WedgemoreUserInput *wui);
	void GetLStickValues(WedgemoreUserInput *wui);
	void GetRStickValues(WedgemoreUserInput *wui);
public:
	UserInterface();
	virtual ~UserInterface();
	void Init(void);
	void GetData(WedgemoreUserInput *);
};

#endif /* SRC_USERINTERFACE_H_ */
