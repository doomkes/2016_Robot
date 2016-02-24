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
	if(m_manStick.GetRawButton(PICKUPPOS)) {
		wui->PickupPos = true;
	}
	if(m_manStick.GetRawButton(STARTPOS)) {
		wui->StartPos = true;
	}
	if(m_manStick.GetRawButton(LOWGOALPOS)) {
		wui->LowGoal = true;
	}
//	if(m_manStick.GetRawButton(AUTOTRACK)) {
//		wui->AutoTrack = true;
//	}
	if(m_manStick.GetRawButton(HIGOALPOS)){
		wui->HiGoal = true;
	}
	if(m_manStick.GetRawButton(SHOOT)) {
		wui->Shoot = true;
	}
	if(m_manStick.GetRawButton(SPINUP)){
		wui->SpinUp = true;
	}
	if(m_manStick.GetRawButton(PICKUP)) {
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
	if(m_lStick.GetRawButton(DROPALL))
		{
			wui->DropFR = true;
			wui->DropBR = true;
			wui->DropFL = true;
			wui->DropBL = true;
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
	wui->SpinUp = false;
	wui->Shoot = false;
	wui->Pickup = false, wui->SpinUp = false, wui->LowGoal = false, wui->HiGoal = false, wui->AutoTrack = false, wui->PickupPos = false, wui->StartPos = false;
	GetLStickValues(wui);
	GetRStickValues(wui);
	GetManStickValues(wui);

	wui->LeftSpeed = m_lStick.GetY();
	wui->RightSpeed = m_rStick.GetY();
}
