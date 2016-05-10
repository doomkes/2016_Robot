/*
 * RobotMap.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Win7
 */

#ifndef SRC_ROBOTMAP_H_
#define SRC_ROBOTMAP_H_

enum ManStickMap {
	LEFTAXIS = 2,
	RIGHTAXIS = 4,
	SHOOT = 7,
	PICKUPPOS = 2,
	SPINUP = 8,
	SLOWSPIN = 6,
	ZERO = 10,
	PICKUP = 4,
	STARTPOS = 3,
	MIDSHOTPOS = 1,//Button to press to shoot from the defenses.,

};

enum LStickMap {
	DROPLEFT = 90,//POV
	DROPRIGHT = 270,//POV
	DROPFRONT = 0,//POV
	DROPBACK = 180,//POV
	DROPALL = 5
};

enum RStickMap {
	REVERSE_DRIVE = 11,
	TOGGLE_RUN_LIGHT = 12,
	CLIMBER = 5
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
