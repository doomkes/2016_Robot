#include "WPILib.h"
#include "UserInterface.h"
#include <stdio.h>
#include <string>
#include "DriveTrain/TankDrive.h"
#include "Shooter.h"
#include "Camera.h"
#include "Autonomous/Cross.h"
#include "Leddar.h"

#include "DriveTrain/SuspensionDrive.h"
//The Robot's name is "Wedgemore"

class Wedgemore: public IterativeRobot
{
private:
	TankDrive m_tank;
	SuspensionDrive m_suspension;
	Shooter m_shooter;
	SendableChooser *autoChooser = nullptr;
	Leddar m_leddar;
	UserInterface ui;
	WedgemoreUserInput wui;
	Camera m_camera;
	Defense selectedDefense = NO_DEFENSE;
public:
	Wedgemore()
	{
	}

	void RobotInit()
	{
		CameraServer::GetInstance()->StartAutomaticCapture("cam1");
		autoChooser = new SendableChooser();
		autoChooser->AddDefault("NO DEFENSE",		(void*)NO_DEFENSE);
		autoChooser->AddObject("LOW BAR",			(void*)LOW_BAR);
		autoChooser->AddObject("PORTCULLIS",		(void*)PORTCULLIS);
		autoChooser->AddObject("RAMPARTS",			(void*)RAMPARTS);
		autoChooser->AddObject("ROCK WALL",			(void*)ROCK_WALL);
		autoChooser->AddObject("MOAT",				(void*)MOAT);
		autoChooser->AddObject("SALLY PORT",		(void*)SALLY_PORT);
		autoChooser->AddObject("DRAW BRIDGE",		(void*)DRAW_BRIDGE);
		autoChooser->AddObject("ROUGH TERRAIN",		(void*)ROUGH_TERRAIN);
		autoChooser->AddObject("CHEVAL DE FRISE",	(void*)SHOVEL_THE_FRIES);
		SmartDashboard::PutData("AUTO", autoChooser);

	}

	void AutonomousInit()
	{
		//m_leddar.StartAutomaticDetections();
		selectedDefense = *(Defense*)autoChooser->GetSelected();
		Cross(selectedDefense, &m_tank, &m_suspension);
	}
	void DisabledInit() {
		//m_leddar.StopAutomaticDetections();
	}
	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{
		SmartDashboard::PutString("Teleop State", "TeleopInit");
		//m_leddar.StartAutomaticDetections();

	}

	void TeleopPeriodic()
	{
		ui.GetData(&wui);
		m_tank.Drive(wui.LeftSpeed, wui.RightSpeed);

		//m_shooter.Lift(wui.LiftSpeed);
		if(wui.StartPos) {
			m_shooter.LiftTo(-14);
		}
		if(wui.PickupPos) {
			m_shooter.LiftTo(-1936);
		}
		if(wui.LowGoal) {
			m_shooter.LiftTo((-1936-14)/3*2+14);
		}
		if(wui.HiGoal) {
			m_shooter.LiftTo((-1936-14)/3+14);
		}
		if(wui.SpinUp) {
			m_shooter.Spinup(12);
		}
		else {
			m_shooter.Stop();
		}
		m_shooter.Shoot(wui.Shoot);
		if(wui.Pickup) {
			m_shooter.Pickup();
		}

		m_suspension.SetFrontLeft(wui.DropFL);
		m_suspension.SetBackLeft(wui.DropBL);
		m_suspension.SetFrontRight(wui.DropFR);
		m_suspension.SetBackRight(wui.DropBR);

	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Wedgemore)
