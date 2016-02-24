/*
 * RobotMap.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Win7
 */

#ifndef SRC_ROBOTMAP_H_
#define SRC_ROBOTMAP_H_

enum ManStickMap {
	SHOOT = 6,
	PICKUP = 5,
	PICKUPPOS = 2,
	LOWGOALPOS = 1,
	HIGOALPOS = 7,
	STARTPOS = 8,
	//AUTOTRACK = 2,
	LIFTAXIS = 1,
	SPINUP = 3
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
