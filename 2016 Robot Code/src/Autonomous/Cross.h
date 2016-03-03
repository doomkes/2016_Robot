/*
 * Cross.h
 *
 *  Created on: Feb 7, 2016
 *      Author: Joey172
 */

#ifndef SRC_AUTONOMOUS_CROSSERS_CROSS_H_
#define SRC_AUTONOMOUS_CROSSERS_CROSS_H_

enum Defense
{
	// Catagory A:
	PORTCULLIS,
	ROCK_WALL,
	// Catagory B:
	RAMPARTS,
	MOAT,
	//Catagory C:
	SALLY_PORT,
	DRAW_BRIDGE,
	// Catagory D:
	ROUGH_TERRAIN,
	CHEVAL_DE_FRISE,
	LOW_BAR,
	NO_DEFENSE,
	SHOVEL_THE_FRIES = CHEVAL_DE_FRISE,
};

class TankDrive;
class SuspensionDrive;

bool Cross(Defense, TankDrive*, SuspensionDrive*);

#endif /* SRC_AUTONOMOUS_CROSSERS_CROSS_H_ */
