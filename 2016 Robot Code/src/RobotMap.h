/*
 * RobotMap.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Win7
 */

#ifndef SRC_ROBOTMAP_H_
#define SRC_ROBOTMAP_H_

#define X3D_RSTICK
enum ManStickMap {
	SHOOT = 5,
	LIMIT_SHOOT = 1,
	PICKUP = 4,
	PICKUPPOS = 9,
	STARTPOS = 10,
	LIFTAXIS = 1,
	SLIDERAXIS = 3,
	YAWAXIS = 2,
	SPINUP = 2,
	SLOWSPIN = 3,
	ZERO = 8,
	CUSTOMSHOT = 33, //Not currently used
	BATTERHIGOALPOS = 11, //Button to press to shoot from the batter.
	DEFENSEHIGOALPOS = 12,//Button to press to shoot from the defenses.
	MIDSHOTPOS = 6,//Button to press to shoot from the defenses.,
	TOGGLE_LIGHT = 7,
	CLIMBER = 0

};
enum LStickMap {
#ifdef X3D_LSTICK	// X3D config.
	DROPLEFT = 3,
	DROPRIGHT = 4,
	DROPFRONT = 6,
	DROPBACK = 5,
	DROPALL = 1,
#else // attack 3 config.
	DROPLEFT = 4,
	DROPRIGHT = 5,
	DROPFRONT = 3,
	DROPBACK = 2,
	DROPALL = 1,

#endif
};
enum RStickMap {
#ifdef X3D_RSTICK
	REVERSE_DRIVE = 1,
	SET_SPEED_MODE = 11,
	SET_VBUS_MODE = 12,
	TOGGLE_RUN_LIGHT = 2,
	SHOOTER_DOWN = -99
#else // attack 3
	REVERSE_DRIVE = 1,
	SET_SPEED_MODE = 10,
	SET_VBUS_MODE = 11,
	TOGGLE_RUN_LIGHT = 2,
	SHOOTER_DOWN = -99
#endif
};
enum DIOMap {
	AIM_LIGHT = 9,
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
