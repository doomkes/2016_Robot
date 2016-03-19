#include "WPILib.h"
#include "UserInterface.h"
#include <stdio.h>
#include <string>
#include "DriveTrain/TankDrive.h"
#include "Shooter.h"
#include "Camera.h"
#include "Leddar.h"
#include "Autonomous.h"
#include "DriveTrain/SuspensionDrive.h"
//The Robot's name is "Wedgemore"
enum ShooterMode {
	STOW_MODE,
	PICKUP_MODE,
	BATTER_HIGOAL_MODE,
	DEFENSE_HIGOAL_MODE
};

class Wedgemore: public IterativeRobot
{
private:
	TankDrive m_tank;
	SuspensionDrive m_suspension;
	Shooter m_shooter;
	Leddar m_leddar;
	UserInterface ui;
	WedgemoreUserInput wui;
	Autonomous m_auto;
	Camera m_camera;
	ShooterMode m_shooterMode = STOW_MODE;
public:
	Wedgemore()
		: m_auto(&m_tank, &m_suspension, &m_shooter){
	}

	void RobotInit()
	{
		CameraServer::GetInstance()->StartAutomaticCapture("cam1");
		SmartDashboard::PutNumber("Driver P", 1);
		SmartDashboard::PutNumber("Driver I", 0);

		SmartDashboard::PutNumber("ShooterSpeed", 12);
		SmartDashboard::PutNumber("ShooterAngle", 0);

		SmartDashboard::PutNumber("lift accel",		1);
		SmartDashboard::PutNumber("lift decel",		1);
		SmartDashboard::PutNumber("lift max speed",	0.5);

		SmartDashboard::PutNumber("Shooter P",		4);
		SmartDashboard::PutNumber("Shooter I",		0);
		SmartDashboard::PutNumber("Shooter D",		0);

		SmartDashboard::PutNumber("Auto Mode Select", 0);
		SmartDashboard::PutNumber("Total Distance", 260);


	}

	void AutonomousInit()
	{
		m_shooter.Zero();
		int automode = SmartDashboard::GetNumber("Auto Mode Select", 0);
		m_auto.Init(automode);

	}

	void AutonomousPeriodic()
	{
		m_auto.Periodic();
		m_shooter.Update();
	}

	void TeleopInit()
	{
		m_shooterMode = STOW_MODE;
		m_tank.SetMode(VBUS_MODE);
		m_shooter.Zero();
		m_tank.Zero();
		ui.Init(&wui);
	}

	void TeleopPeriodic()
	{

		ui.GetData(&wui);
//		float LeftSpeed, RightSpeed;
//		if(wui.GiveManControl) {
//			m_tank.Drive(wui.YawValue, -wui.YawValue);
//		}
		//m_shooter.ShooterLiftZero();
		m_tank.Drive(wui.LeftSpeed, wui.RightSpeed);

//		if (fabs(wui.LiftSpeed) < 0.05) {
//			wui.LiftSpeed = 0;
//		}

//		if(wui.SpinAndShoot) {
//			m_shooter.SpinShoot();
//		}
		if(wui.PickupPos) {
			m_shooterMode = PICKUP_MODE;
		}
		if(wui.StartPosition) {	//start & stow pos
			m_shooterMode = STOW_MODE;
		}
		if(wui.BatterHiGoal) {
			m_shooterMode = BATTER_HIGOAL_MODE;
		}
		if(wui.DefenseHiGoal) {
			m_shooterMode = DEFENSE_HIGOAL_MODE;
		}
		if(wui.SpinUp) {
			m_shooter.Spinup(SmartDashboard::GetNumber("ShooterSpeed", 0));
		}
		if(wui.SpinUpLow) {
			m_shooter.Spinup(7);
		}
		else {
			m_shooter.Stop();
		}
		m_shooter.Shoot(wui.Shoot);

		if(wui.Pickup) {
			m_shooter.Pickup();
		}
//		if(wui.Custom) {
//			m_shooter.LiftTo(SmartDashboard::GetNumber("ShooterAngle", 0));
//		}
		if(wui.Zero) {
			m_shooterMode = STOW_MODE;
			m_shooter.Zero();
			//m_shooter.shooter_zero = 1;
		}
		if(wui.ToggleLight) {
			m_shooter.ToggleLight();
		}
		if(wui.SpeedMode) {
			m_tank.SetMode(SPEED_MODE);
			SmartDashboard::PutBoolean("VBus Mode", false);
		}
		if(wui.VBusMode) {
			m_tank.SetMode(VBUS_MODE);
			SmartDashboard::PutBoolean("VBus Mode", true);
		}
		m_tank.Reverse(wui.ReverseDrive);
		SmartDashboard::PutNumber("slider val", wui.SliderValue);
		float AngleAdjust = wui.LiftSpeed * (-(wui.SliderValue) + 1);
		switch(m_shooterMode) {
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
				m_shooter.LiftTo(36.6 + AngleAdjust*20); //TODO use preferences for values.
				break;
		}

		m_shooter.Update();

		m_suspension.SetFrontLeft(wui.DropFL);
		m_suspension.SetBackLeft(wui.DropBL);
		m_suspension.SetFrontRight(wui.DropFR);
		m_suspension.SetBackRight(wui.DropBR);
	}

	void TestPeriodic()
	{

	}
	void DisabledInit(){
		m_tank.Zero();
	}
};

START_ROBOT_CLASS(Wedgemore)
