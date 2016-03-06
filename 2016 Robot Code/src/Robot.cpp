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
		SmartDashboard::PutNumber("Driver P", 0);

		SmartDashboard::PutNumber("ShooterSpeed", 12);
		SmartDashboard::PutNumber("ShooterAngle", 0);

		SmartDashboard::PutNumber("lift accel",		0.5);
		SmartDashboard::PutNumber("lift decel",		1);
		SmartDashboard::PutNumber("lift max speed",	0.25);

		SmartDashboard::PutNumber("Shooter P",		1);
		SmartDashboard::PutNumber("Shooter I",		0);
		SmartDashboard::PutNumber("Shooter D",		0);

		m_shooter.Zero();
	}

	void AutonomousInit()
	{
	}

	void AutonomousPeriodic()
	{
	}

	void TeleopInit()
	{
		m_tank.Zero();
	}
	enum ShooterMode {
		STOW_MODE,
		PICKUP_MODE,
		BATTER_HIGOAL_MODE,
		DEFENSE_HIGOAL_MODE
	};
	void TeleopPeriodic()
	{
		static ShooterMode shooterMode = STOW_MODE;
		ui.GetData(&wui);
		m_tank.Drive(wui.LeftSpeed, wui.RightSpeed);

//		if (fabs(wui.LiftSpeed) < 0.05) {
//			wui.LiftSpeed = 0;
//		}

		if(wui.SpinAndShoot) {
			m_shooter.SpinShoot();
		}
		if(wui.PickupPos) {
			shooterMode = PICKUP_MODE;
		}
		if(wui.StartPosition) {	//start & stow pos
			shooterMode = STOW_MODE;
		}
		if(wui.BatterHiGoal) {
			shooterMode = BATTER_HIGOAL_MODE;
		}
		if(wui.SpinUp) {
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
		SmartDashboard::PutNumber("slider val", wui.SliderValue);
		float AngleAdjust = wui.LiftSpeed * (-(wui.SliderValue) + 1);
		switch(shooterMode) {
			case STOW_MODE:
				m_shooter.LiftTo(AngleAdjust * 30);
				break;
			case PICKUP_MODE:
				m_shooter.LiftTo(180 - AngleAdjust * 35);//TODO use preferences for values.
				break;
			case BATTER_HIGOAL_MODE:
				m_shooter.LiftTo(45 + AngleAdjust*20); //TODO use preferences for values.
				break;
			case DEFENSE_HIGOAL_MODE:
				m_shooter.LiftTo(30 + AngleAdjust*20); //TODO use preferences for values.
				break;
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
	void DisabledInit(){
		m_tank.Zero();
	}
};

START_ROBOT_CLASS(Wedgemore)
