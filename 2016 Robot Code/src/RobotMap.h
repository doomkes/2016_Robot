/*
 * RobotMap.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Win7
 */

#ifndef SRC_ROBOTMAP_H_
#define SRC_ROBOTMAP_H_

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
	TOGGLE_LIGHT = 7

};

enum LStickMap {
	DROPLEFT = 4,
	DROPRIGHT = 5,
	DROPFRONT = 3,
	DROPBACK = 2,
	DROPALL = 1,
};

enum DIOMap {
	AIM_LIGHT = 9,
	RUN_GUN_LIGHT = 8
};
enum RStickMap {
	REVERSE_DRIVE = 1,
	SET_SPEED_MODE = 10,
	SET_VBUS_MODE = 11,
	TOGGLE_RUN_LIGHT = 2,
	SHOOTER_DOWN = -99,
	CLIMBER = 5
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
