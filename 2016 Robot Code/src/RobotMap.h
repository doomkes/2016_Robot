/*
 * RobotMap.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Win7
 */

#ifndef SRC_ROBOTMAP_H_
#define SRC_ROBOTMAP_H_

enum ManStickMap {
	SHOOT = 1,
	PICKUP = 4,
	PICKUPPOS = 9,
	STARTPOS = 10,
	LIFTAXIS = 1,
	SLIDERAXIS = 3,
	SPINUP = 2,
	ZERO = 7,
	CUSTOMSHOT = 8,
	BATTERHIGOALPOS = 11, //Button to press to shoot from the batter.
	DEFENSEHIGOALPOS = 12,//Button to press to shoot from the defenses.
	SPINANDSHOOT = 4,
};

enum LStickMap {
	DROPLEFT = 4,
	DROPRIGHT = 5,
	DROPFRONT = 3,
	DROPBACK = 2,
	DROPALL = 1,
};

enum RStickMap {
	DROPFRONTRIGHT = 3,
	DROPBACKRIGHT = 5,
	DROPFRONTLEFT = 4,
	DROPBACKLEFT = 2,
};

enum SolenoidMap {
	FRONTLEFTSOL,
	FRONTRIGHTSOL,
	BACKLEFTSOL,
	BACKRIGHTSOL,
	SHOOTERSOL,
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
