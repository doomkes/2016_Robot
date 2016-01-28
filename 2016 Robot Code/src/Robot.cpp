#include "WPILib.h"
#include "UserInterface.h"
#include <stdio.h>
#include <unistd.h>
#include <ImageProcessing.h>

//The Robot's name is "Wedgemore"
//auto grip = NetworkTable::GetTable("grip");
class Robot: public IterativeRobot
{

	RobotDrive m_tank;	//normal drive wheels tank drive
	CANTalon m_turretLazy, m_turretLift, m_turretShoot1, m_turretShoot2;
	Joystick m_driveStickL, m_driveStickR, m_turretStick; //control joysticks
	Solenoid m_driveFR, m_driveFL, m_driveRR, m_driveRL; //solenoids that control drive wheel height
	PowerDistributionPanel PDBoard;
	Encoder m_lazyCode, m_liftCode, m_shoot1Code, m_shoot2Code;
	DigitalInput m_homeLiftSwitch, m_lazySwitchF, m_lazySwitchR, m_backLiftSwitch;

private:


	UserInterface ui;
	struct WedgemoreUserInput wui;
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

public:

	Robot():
		m_tank(0, 4, 2, 6),
		m_turretLazy(1),
		m_turretLift(2),
		m_turretShoot1(3),
		m_turretShoot2(4),
		m_driveStickL(1),
		m_driveStickR(2),
		m_turretStick(3),
		m_driveFR(0),
		m_driveFL(0),
		m_driveRR(0),
		m_driveRL(0),
		PDBoard(0),
		m_lazyCode(0),
		m_liftCode(0),
		m_shoot1Code(0),
		m_shoot2Code(0),
		m_homeLiftSwitch(0),
		m_lazySwitchF(0),
		m_lazySwitchR(0),
		m_backLiftSwitch(0)

booblyboo

{}


	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);
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
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void AutonomousPeriodic()
	{

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		ui.GetData(&wui);
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
