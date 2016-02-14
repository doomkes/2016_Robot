/*
 * UserInterface.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: James
 */

#include <UserInterface.h>
#include <array>
#include "robotmap.h"
//The Robot's name is "Wedgemore"

UserInterface::UserInterface() :
		m_lStick(0), m_rStick(1), m_manStick(2)
{
}

UserInterface::~UserInterface()
{

}
void UserInterface::GetManStickValues(WedgemoreUserInput *wui){
	wui->RotateSpeed = m_manStick.GetRawAxis(4);
	wui->LiftSpeed = m_manStick.GetRawAxis(1);
	if(m_manStick.GetRawButton(0)) {
		wui->PickupPosition = true;
	}
	if(m_manStick.GetRawButton(1)) {
		wui->LowGoal = true;
	}
	if(m_manStick.GetRawButton(2)) {
		wui->AutoTrack = true;
	}
	if(m_manStick.GetRawButton(3)){
		wui->HiGoal = true;
	}
	if(m_manStick.GetRawButton(4)) {
		wui->Shoot = true;
	}
	if(m_manStick.GetRawButton(5)) {
		wui->Pickup = true;
	}
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
}
void UserInterface::GetRStickValues(WedgemoreUserInput *wui){
	if(m_rStick.GetRawButton(DROPBACKRIGHT))
	{
		wui->DropBR = true;
	}
	if(m_rStick.GetRawButton(DROPBACKLEFT))
	{
		wui->DropBL = true;
	}
	if(m_rStick.GetRawButton(DROPFRONTLEFT))
	{
		wui->DropFL = true;
	}
	if(m_rStick.GetRawButton(DROPFRONTRIGHT))
	{
		wui->DropFR = true;
	}
}

void UserInterface::GetData(WedgemoreUserInput *wui)
{
	wui->DropFL = false;
	wui->DropFR = false;
	wui->DropBL = false;
	wui->DropBR = false;

	GetLStickValues(wui);
	GetRStickValues(wui);
	GetManStickValues(wui);

	wui->LeftSpeed = m_lStick.GetY();
	wui->RightSpeed = m_rStick.GetY();
}
