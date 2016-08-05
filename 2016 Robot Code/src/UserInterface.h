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
	float LiftSpeed = 0;
	float SliderValue = 0;
	float YawValue = 0.0;
	bool Shoot = false, Pickup = false;
	bool SpinUp = false, SpinUpLow = false;
	bool StartPosition = false;
	bool AutoTrack = false;
	bool PickupPos = false;
	bool Zero = false;
	bool Custom = false;
	bool ShooterDown = false;
	bool BatterHiGoal = false;
	bool DefenseHiGoal = false;
	bool MidHiGoal = false;
	bool LimitShoot = false;
	bool SpeedMode = false;
	bool VBusMode = false;
	bool RunGunLight = false;
	bool ReverseDrive = false;
	int Climber = -1;
	float SpeedMultiplier = 1.0;
};

class UserInterface
{
private:
	Joystick m_lStick, m_rStick, m_manStick, m_overrideControl;
	void GetManStickValues(WedgemoreUserInput *wui);
	void GetLStickValues(WedgemoreUserInput *wui);
	void GetRStickValues(WedgemoreUserInput *wui);
public:
	UserInterface();
	virtual ~UserInterface();
	void Init(WedgemoreUserInput *wui);
	void GetData(WedgemoreUserInput *);
};

#endif /* SRC_USERINTERFACE_H_ */
