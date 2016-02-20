/*
 * RobotMap.h
 *
 *  Created on: Feb 13, 2016
 *      Author: Win7
 */

#ifndef SRC_ROBOTMAP_H_
#define SRC_ROBOTMAP_H_

enum ManStickMap {
	HOME = 0,
	SHOOT = 4,
	PICKUP = 5,
	LOWGOAL = 1,
	HIGOAL = 4,
	AUTOTRACK = 2,
	LIFTAXIS = 1,
	ROTATEAXIS = 4,
	SPINUP = 3
};

enum LStickMap {
	DROPLEFT = 4,
	DROPRIGHT = 5,
	DROPFRONT = 3,
	DROPBACK = 2,
};

enum RStickMap {
	DROPFRONTRIGHT = 5,
	DROPBACKRIGHT = 2,
	DROPFRONTLEFT = 4,
	DROPBACKLEFT = 3,
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
	SUSAN,
	LIFT,
	SHOOT1,
	SHOOT2,
};
#endif /* SRC_ROBOTMAP_H_ */
