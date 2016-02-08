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

};

class UserInterface
{
private:
	Joystick m_lStick, m_rStick, m_manStick;
public:
	UserInterface();
	virtual ~UserInterface();
	void Init(void);
	void GetData(WedgemoreUserInput *);
};

#endif /* SRC_USERINTERFACE_H_ */
