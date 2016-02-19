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

public:
	Robot()
	{

	}

	void RobotInit()
	{

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
	}

	void AutonomousPeriodic()
	{
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
//		ui.GetData(&wui);
//		m_tank.Drive(wui.LeftSpeed, wui.RightSpeed);
//
//		m_shooter.Rotate(wui.RotateSpeed*3); //70 degrees per second at full value
//		m_shooter.Lift(wui.LiftSpeed*1.193); //4 seconds for 180 degree revolution
//		if(wui.SpinUp) {
//			m_shooter.Spinup(1);
//		}
//		if(wui.Shoot) {
//			m_shooter.Shoot();
//		}
//		if(wui.Pickup) {
//			m_shooter.Pickup();
//		}
//
//		m_suspension.SetFrontLeft(wui.DropFL);
//		m_suspension.SetBackLeft(wui.DropBL);
//		m_suspension.SetFrontRight(wui.DropFR);
//		m_suspension.SetBackRight(wui.DropBR);

		m_leddar.GetDetections();
//		m_shooter.Update();
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Robot)
