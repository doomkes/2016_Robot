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

class Robot: public IterativeRobot
{
private:
	TankDrive m_tank;
	SuspensionDrive m_suspension;
	Shooter m_shooter;
	Leddar m_leddar;
	UserInterface ui;
	WedgemoreUserInput wui;
	//Joystick m_lStick, m_rStick, m_manStick;
	Camera m_camera;

	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser = nullptr;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;
public:
	Robot()
	{

	}

	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);
		SmartDashboard::PutString("hello", "I'm here");
		//m_leddar.GetRawDetections();
	}

	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{
		autoSelected = *((std::string*)chooser->GetSelected());
		std::string autoSelected = SmartDashboard::GetString("Auto Selector",
				autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom)
		{
			//Custom Auto goes here
		} else
		{
			//Default Auto goes here
		}
	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom)
		{
			//Custom Auto goes here
		} else
		{
			//Default Auto goes here
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		ui.GetData(&wui);
		m_tank.Drive(wui.LeftSpeed, wui.RightSpeed);

//		m_camera.AutoTrack();

		m_suspension.SetFrontLeft(wui.DropFL);
		m_suspension.SetBackLeft(wui.DropBL);
		m_suspension.SetFrontRight(wui.DropFR);
		m_suspension.SetBackRight(wui.DropBR);
		cout << "stuff is happening" << endl;
		SmartDashboard::PutString("hi", "teleop periodic");
		m_leddar.GetDetections();

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
