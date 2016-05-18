/*
 * RobotMap.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Win7
 */

#ifndef SRC_ROBOTMAP_H_
#define SRC_ROBOTMAP_H_

enum JoystickMap {
	//Button
	MIDSHOTPOS = 1,//Button to press to shoot from the defenses.,
	PICKUPPOS = 2,
	STARTPOS = 3,
	SLOWSPIN = 4,
	DROPALL = 5,
	PICKUP = 6,
	SHOOT = 7,
	SPINUP = 8,
	CLIMBER = 9,
	ZERO = 10,
	REVERSE_DRIVE = 11,
	TOGGLE_RUN_LIGHT = 12,

	//Axis
	LEFTAXIS = 1,
	RIGHTAXIS = 3,

	//POV
	DROPLEFT = 90,//POV
	DROPRIGHT = 270,//POV
	DROPFRONT = 0,//POV
	DROPBACK = 180,//POV
};

enum DIOMap {
	RUN_GUN_LIGHT = 8
};

enum SolenoidMap {
	FRONTLEFTSOL,
	FRONTRIGHTSOL,
	BACKLEFTSOL,
	BACKRIGHTSOL,
	SHOOTERSOL = 7,
};

enum CANTalonMap {
	RIGHTDRIVE1,
	RIGHTDRIVE2,
	LEFTDRIVE1,
	LEFTDRIVE2,
	LIFT,
	SHOOT1,
	SHOOT2,
};
#endif /* SRC_ROBOTMAP_H_ */
