#include "Cross.h"
#include "DriveTrain/TankDrive.h"
#include "DriveTrain/SuspensionDrive.h"

bool CrossLowBar(TankDrive*, SuspensionDrive*);
bool CrossPortcullis(TankDrive*, SuspensionDrive*);
bool CrossRockWall(TankDrive*, SuspensionDrive*);
bool CrossRamparts(TankDrive*, SuspensionDrive*);
bool CrossMote(TankDrive*, SuspensionDrive*);
bool CrossSallyPort(TankDrive*, SuspensionDrive*);
bool CrossDrawBridge(TankDrive*, SuspensionDrive*);
bool CrossRoughTerrain(TankDrive*, SuspensionDrive*);
bool CrossChevalDeFrise(TankDrive*, SuspensionDrive*);

bool Cross(Defense defense, TankDrive *tank, SuspensionDrive *suspension)
{
	switch(defense)
	{
		case LOW_BAR:
			//return CrossLowBar(tank, suspension);
		case PORTCULLIS:
			//return CrossPortcullis(tank, suspension);
		case ROCK_WALL:
			//return CrossRockWall(tank, suspension);
		case RAMPARTS:
			//return CrossRamparts(tank, suspension);
		case MOTE:
			//return CrossMote(tank, suspension);
		case SALLY_PORT:
			//return CrossSallyPort(tank, suspension);
		case DRAW_BRIDGE:
			//return CrossDrawBridge(tank, suspension);
		case ROUGH_TERRAIN:
			//return CrossRoughTerrain(tank, suspension);
		case CHEVAL_DE_FRISE:
			//return CrossChevalDeFrise(tank, suspension);
		default:
			return false;
	}
	return false;
}

