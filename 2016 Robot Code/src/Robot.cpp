#include "WPILib.h"
#include "UserInterface.h"
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <ImageProcessing.h>
#include "TankDrive.h"
#include "Shooter.h"
#include "Leddar.h"
#include "SuspensionDrive.h"
//The Robot's name is "Wedgemore"
//auto grip = NetworkTable::GetTable("grip");

class Robot: public IterativeRobot
{
private:
	TankDrive m_tank;
	SuspensionDrive m_suspension;
	Shooter m_shooter;
	Leddar m_leddar;
	Joystick m_driveStickL, m_driveStickR, m_manStick; //control joysticks
	UserInterface ui;
	WedgemoreUserInput wui;
	//PowerDistributionPanel PDBoard;
	//Encoder m_lazyCode, m_liftCode, m_shoot1Code, m_shoot2Code;
	//DigitalInput m_homeLiftSwitch, m_lazySwitchF, m_lazySwitchR, m_backLiftSwitch;

    const char *JAVA = "/usr/local/frc/JRE/bin/java";
    char *GRIP_ARGS[5] = { "java", "-jar", "/home/lvuser/grip.jar",
            "/home/lvuser/project.grip", NULL };

	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser = nullptr;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

public:
	Robot():
		m_driveStickL(0),
		m_driveStickR(1),
		m_manStick(2)
//		PDBoard(0),
//		m_lazyCode(0),
//		m_liftCode(0),
//		m_shoot1Code(0),
//		m_shoot2Code(0),
//		m_homeLiftSwitch(0),
//		m_lazySwitchF(0),
//		m_lazySwitchR(0)
//		m_backLiftSwitch(0)
	{

	}

	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);

		m_leddar.GetRawDetections();

        if (fork() == 0) {
            if (execv(JAVA, GRIP_ARGS) == -1) {
                perror("Error running GRIP");
            }
        }

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
		std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
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

        auto grip = NetworkTable::GetTable("GRIP");

        /* Get published values from GRIP using NetworkTables */
        auto areas = grip->GetNumberArray("targets/area", llvm::ArrayRef<double>());
        static int i = 0;

       if ( i++ % 10 == 0) {
			std::cout << "areas size:" << areas.size() << std::endl;

			for (auto area : areas) {
				std::cout << "Got contour with area=" << area << std::endl;
			}
       }

	}

	void TeleopInit()
	{
	}

	void TeleopPeriodic()
	{
		m_tank.Drive(m_driveStickL.GetY(), m_driveStickR.GetY());

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)
