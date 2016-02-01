/*
 * UserInterface.h
 *
 *  Created on: Jan 20, 2016
 *      Author: James
 */

#ifndef SRC_USERINTERFACE_H_
#define SRC_USERINTERFACE_H_

#include "WPILib.h"

//The Robot's name is "Wedgemore"

struct WedgemoreUserInput{



};

class UserInterface {
private:
		Joystick lStick, rStick, shootStick;

public:
	UserInterface();
	virtual ~UserInterface();
	void Init(void);
	void GetData(WedgemoreUserInput *);

};

#endif /* SRC_USERINTERFACE_H_ */
