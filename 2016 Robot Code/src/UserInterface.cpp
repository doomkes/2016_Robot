/*
 * UserInterface.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: James
 */

#include <UserInterface.h>
#include <array>
//The Robot's name is "Wedgemore"

UserInterface::UserInterface() :
		m_lStick(0), m_rStick(1), m_manStick(2)
{
}

UserInterface::~UserInterface()
{

}

void UserInterface::GetData(WedgemoreUserInput *wui)
{
	wui->DropFL = false;
	wui->DropFR = false;
	wui->DropBL = false;
	wui->DropBR = false;
	/* GET LSTICK VALUES */
	if(m_lStick.GetRawButton(2))
	{
		wui->DropBR = true;
		wui->DropBL = true;
	}
	if(m_lStick.GetRawButton(3))
	{
		wui->DropFL = true;
		wui->DropBL = true;
	}
	if(m_lStick.GetRawButton(4))
	{
		wui->DropFL = true;
		wui->DropBL = true;
	}
	if(m_lStick.GetRawButton(5))
	{
		wui->DropFR = true;
		wui->DropBR = true;
	}

	/* GET RSTICK VALUES */
	if(m_rStick.GetRawButton(2))
	{
		wui->DropBR = true;
	}
	if(m_rStick.GetRawButton(3))
	{
		wui->DropBL = true;
	}
	if(m_rStick.GetRawButton(4))
	{
		wui->DropFL = true;
	}
	if(m_rStick.GetRawButton(5))
	{
		wui->DropFR = true;
	}

	wui->LeftSpeed = m_lStick.GetY();
	wui->RightSpeed = m_rStick.GetY();
}
