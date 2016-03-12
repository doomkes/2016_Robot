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
		m_lStick(0), m_rStick(1), m_manStick(2)
{
}

UserInterface::~UserInterface()
{

}
void UserInterface::Init(WedgemoreUserInput *wui) {
	wui->DropFR = false, wui->DropFL = false, wui->DropBR = false, wui->DropBL = false;
	wui->LeftSpeed = 0.0, wui->RightSpeed = 0.0;
	wui->LiftSpeed = 0;
	wui->SliderValue = 0;
	wui->Shoot = false, wui->Pickup = false;
	wui->SpinUp = false, wui->SpinUp = false;
	wui->ReverseDrive = false;
	wui->StartPosition = false;
	wui->AutoTrack = false;
	wui->PickupPos = false;
	wui->Zero = false;
	wui->Custom = false;
	wui->BatterHiGoal = false;
	wui->DefenseHiGoal = false;
	wui->SpinAndShoot = false;
	wui->SpeedMode = false;
	wui->VBusMode = false;
	wui->ToggleLight = false;
	wui->GiveManControl = false;
}

void UserInterface::GetManStickValues(WedgemoreUserInput *wui){
	static bool lastLightButton = false;
	wui->LiftSpeed = m_manStick.GetRawAxis(LIFTAXIS);
	wui->SliderValue = m_manStick.GetRawAxis(SLIDERAXIS);
	wui->YawValue = 0;

	wui->PickupPos = m_manStick.GetRawButton(PICKUPPOS);
	wui->StartPosition = m_manStick.GetRawButton(STARTPOS);
	wui->Shoot = m_manStick.GetRawButton(SHOOT);
	wui->SpinUp = m_manStick.GetRawButton(SPINUP);
	wui->SpinUp = m_manStick.GetRawButton(SLOWSPIN);
	wui->Pickup = m_manStick.GetRawButton(PICKUP);
	wui->Zero = m_manStick.GetRawButton(ZERO);
	wui->Custom = m_manStick.GetRawButton(CUSTOMSHOT);
	wui->BatterHiGoal = m_manStick.GetRawButton(BATTERHIGOALPOS);
	wui->DefenseHiGoal = m_manStick.GetRawButton(DEFENSEHIGOALPOS);
	wui->SpinAndShoot = m_manStick.GetRawButton(SPINANDSHOOT);
	wui->ToggleLight = m_manStick.GetRawButton(TOGGLE_LIGHT) && !lastLightButton ? true : false;
	lastLightButton = m_manStick.GetRawButton(TOGGLE_LIGHT);
}
void UserInterface::GetLStickValues(WedgemoreUserInput *wui) {
	if(m_lStick.GetRawButton(DROPBACK))
	{
		wui->DropBR = true;
		wui->DropBL = true;
	}
	if(m_lStick.GetRawButton(DROPFRONT))
	{
		wui->DropFL = true;
		wui->DropFR = true;
	}
	if(m_lStick.GetRawButton(DROPLEFT))
	{
		wui->DropFL = true;
		wui->DropBL = true;
	}
	if(m_lStick.GetRawButton(DROPRIGHT))
	{
		wui->DropFR = true;
		wui->DropBR = true;
	}
	if(m_lStick.GetRawButton(DROPALL))
	{
		wui->DropFR = true;
		wui->DropBR = true;
		wui->DropFL = true;
		wui->DropBL = true;
	}
}
void UserInterface::GetRStickValues(WedgemoreUserInput *wui){
//	if(m_rStick.GetRawButton(DROPBACKRIGHT))
//	{
//		wui->DropBR = true;
//	}
//	if(m_rStick.GetRawButton(DROPBACKLEFT))
//	{
//		wui->DropBL = true;
//	}
//	if(m_rStick.GetRawButton(DROPFRONTLEFT))
//	{
//		wui->DropFL = true;
//	}
//	if(m_rStick.GetRawButton(DROPFRONTRIGHT))
//	{
//		wui->DropFR = true;
//	}
	wui->GiveManControl = m_rStick.GetRawButton(GIVE_MAN_CONTROL);
	wui->ReverseDrive = m_rStick.GetRawButton(REVERSE_DRIVE);
	wui->SpeedMode = m_rStick.GetRawButton(SET_SPEED_MODE);
	wui->VBusMode = m_rStick.GetRawButton(SET_VBUS_MODE);
}

void UserInterface::GetData(WedgemoreUserInput *wui)
{
	wui->DropFR = false, wui->DropFL = false, wui->DropBR = false, wui->DropBL = false;
	GetLStickValues(wui);
	GetRStickValues(wui);
	GetManStickValues(wui);

	wui->LeftSpeed = m_lStick.GetY();
	wui->RightSpeed = m_rStick.GetY();
}
