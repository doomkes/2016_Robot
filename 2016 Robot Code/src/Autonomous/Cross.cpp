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
		case NO_DEFENSE:
			SmartDashboard::PutString("selected auto:", "none");
		break;
		case LOW_BAR:
			SmartDashboard::PutString("selected auto:", "none");
			//return CrossLowBar(tank, suspension);
			break;
		case PORTCULLIS:
			SmartDashboard::PutString("selected auto:", "none");
			//return CrossPortcullis(tank, suspension);
			break;
		case ROCK_WALL:
			SmartDashboard::PutString("selected auto:", "none");
			//return CrossRockWall(tank, suspension);
			break;
		case RAMPARTS:
			SmartDashboard::PutString("selected auto:", "none");
			//return CrossRamparts(tank, suspension);
			break;
		case MOAT:
			SmartDashboard::PutString("selected auto:", "none");
			//return CrossMote(tank, suspension);
			break;
		case SALLY_PORT:
			SmartDashboard::PutString("selected auto:", "none");
			//return CrossSallyPort(tank, suspension);
			break;
		case DRAW_BRIDGE:
			SmartDashboard::PutString("selected auto:", "none");
			//return CrossDrawBridge(tank, suspension);
			break;
		case ROUGH_TERRAIN:
			SmartDashboard::PutString("selected auto:", "none");
			//return CrossRoughTerrain(tank, suspension);
			break;
		case CHEVAL_DE_FRISE:
			SmartDashboard::PutString("selected auto:", "none");
			//return CrossChevalDeFrise(tank, suspension);
			break;
		default:
			return false;
	}
	return false;
}

