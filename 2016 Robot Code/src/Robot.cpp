#include "WPILib.h"
#include "UserInterface.h"
#include <stdio.h>
#include <string>
#include "DriveTrain/TankDrive.h"
#include "Shooter.h"
#include "Camera.h"
#include "Leddar.h"
#include "DriveTrain/SuspensionDrive.h"
//The Robot's name is "Wedgemore"

class Wedgemore: public IterativeRobot
{
private:
	TankDrive m_tank;
	SuspensionDrive m_suspension;
	Shooter m_shooter;
	Leddar m_leddar;
	UserInterface ui;
	WedgemoreUserInput wui;
	Camera m_camera;


public:
	Wedgemore()
	{
	}

	void RobotInit()
	{
		SmartDashboard::PutNumber("ShooterSpeed", 12);
		SmartDashboard::PutNumber("ShooterAngle", 0);

		SmartDashboard::PutNumber("lift accel",		0.5);
		SmartDashboard::PutNumber("lift decel",		1);
		SmartDashboard::PutNumber("lift max speed",	0.25);

		SmartDashboard::PutNumber("Shooter P",		0);
		SmartDashboard::PutNumber("Shooter I",		0);
		SmartDashboard::PutNumber("Shooter D",		0);
	}

	void AutonomousInit()
	{
	}

	void AutonomousPeriodic()
	{
	}

	void TeleopInit()
	{
	}

	void TeleopPeriodic()
	{
		//SmartDashboard::PutNumber("position", m_shooter.GetLiftPosition());
		m_shooter.ShooterLiftZero();
		ui.GetData(&wui);
		m_tank.Drive(wui.LeftSpeed, wui.RightSpeed);
		if ((wui.LiftSpeed > 0.1) || (wui.LiftSpeed < -0.1)) {	//manual down
			m_shooter.LiftTo(m_shooter.GetLiftPosition() + wui.LiftSpeed * 0.003 );
		}
		if(wui.PickupPos) {
			m_shooter.LiftTo(0);
		}
		if(wui.StartPosition) {
			m_shooter.LiftTo(-0.5);
		}

		if(wui.SpinUp) {
//			m_shooter.Spinup(12);
			m_shooter.Spinup(SmartDashboard::GetNumber("ShooterSpeed", 0));
		}
		else {
			m_shooter.Stop();
		}
		m_shooter.Shoot(wui.Shoot);
		if(wui.Pickup) {

			m_shooter.Pickup();
		}
		if(wui.Custom) {
			m_shooter.LiftTo(SmartDashboard::GetNumber("ShooterAngle", 0));
		}
		if(wui.Zero) {
			m_shooter.shooter_zero = 1;
		}

		m_shooter.Update();

		m_suspension.SetFrontLeft(wui.DropFL);
		m_suspension.SetBackLeft(wui.DropBL);
		m_suspension.SetFrontRight(wui.DropFR);
		m_suspension.SetBackRight(wui.DropBR);

		//m_leddar.GetDetections();
		//THE DELAY IN LEDDAR.CPP IS CAUSING TOO MUCH LAG ON THE ROBOT DRIVE.
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Wedgemore)
