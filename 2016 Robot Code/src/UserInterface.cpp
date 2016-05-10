/*
 * UserInterface.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: James
 */

#include <UserInterface.h>
#include "robotmap.h"
//The Robot's name is "Wedgemore"

UserInterface::UserInterface() :
		m_fullStick(0)
{
}

UserInterface::~UserInterface()
{

}

void UserInterface::Init(WedgemoreUserInput *wui) {
	wui->DropFR = false, wui->DropFL = false, wui->DropBR = false, wui->DropBL = false;
	wui->LeftSpeed = 0.0, wui->RightSpeed = 0.0;
	wui->Shoot = false, wui->Pickup = false;
	wui->SpinUp = false, wui->SpinUpLow = false;
	wui->ReverseDrive = false;
	wui->StartPosition = false;
	wui->AutoTrack = false;
	wui->PickupPos = false;
	wui->Zero = false;
	wui->MidHiGoal = false;
	wui->RunGunLight = false;
}

void UserInterface::GetAllStickValues(WedgemoreUserInput *wui){
	wui->LeftSpeed = m_fullStick.GetRawAxis(LEFTAXIS);
	wui->RightSpeed = m_fullStick.GetRawAxis(RIGHTAXIS);
	wui->PickupPos = m_fullStick.GetRawButton(PICKUPPOS);
	wui->StartPosition = m_fullStick.GetRawButton(STARTPOS);
	wui->Shoot = m_fullStick.GetRawButton(SHOOT);
	wui->SpinUp = m_fullStick.GetRawButton(SPINUP);
	wui->SpinUpLow = m_fullStick.GetRawButton(SLOWSPIN);
	wui->Pickup = m_fullStick.GetRawButton(PICKUP);
	wui->Zero = m_fullStick.GetRawButton(ZERO);
	wui->MidHiGoal = m_fullStick.GetRawButton(MIDSHOTPOS);

	if(m_fullStick.GetPOV(DROPBACK))
	{
		wui->DropBR = true;
		wui->DropBL = true;
	}
	if(m_fullStick.GetPOV(DROPFRONT))
	{
		wui->DropFL = true;
		wui->DropFR = true;
	}
	if(m_fullStick.GetPOV(DROPLEFT))
	{
		wui->DropFL = true;
		wui->DropBL = true;
	}
	if(m_fullStick.GetPOV(DROPRIGHT))
	{
		wui->DropFR = true;
		wui->DropBR = true;
	}
	if(m_fullStick.GetRawButton(DROPALL))
	{
		wui->DropFR = true;
		wui->DropBR = true;
		wui->DropFL = true;
		wui->DropBL = true;
	}

	static bool lastLightButton = false;
	wui->RunGunLight = m_fullStick.GetRawButton(TOGGLE_RUN_LIGHT) && !lastLightButton ? true : false;
	lastLightButton = m_fullStick.GetRawButton(TOGGLE_RUN_LIGHT);

	wui->RunGunLight = m_fullStick.GetRawButton(TOGGLE_RUN_LIGHT);

	wui->ReverseDrive = m_fullStick.GetRawButton(REVERSE_DRIVE);
	wui->Climber = m_fullStick.GetRawButton(CLIMBER);

	wui->LeftSpeed = m_fullStick.GetRawAxis(LEFTAXIS);
	wui->RightSpeed = m_fullStick.GetRawAxis(RIGHTAXIS);
}

void UserInterface::GetData(WedgemoreUserInput *wui)
{
	wui->DropFR = false, wui->DropFL = false, wui->DropBR = false, wui->DropBL = false;
	GetAllStickValues(wui);

}
