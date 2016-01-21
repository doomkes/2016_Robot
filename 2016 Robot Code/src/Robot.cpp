#include "WPILib.h"
#include "UserInterface.h"
#include <stdio.h>
#include <unistd.h>

//The Robot's name is "Wedgemore"
auto grip = NetworkTable::GetTable("grip");
class Robot: public IterativeRobot
{
private:
	const char *JAVA = "/usr/local/frc/JRE/bin/java";
	    char *GRIP_ARGS[5] = { "java", "-jar", "/home/lvuser/grip.jar",
	            "/home/lvuser/project.grip", NULL };


	UserInterface ui;
	struct WedgemoreUserInput wui;
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	void RobotInit()
	{
		if(execv(JAVA, GRIP_ARGS) == -1) {
			perror("Error running GRIP");
		}
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
	    /* Get published values from GRIP using NetworkTables */
	    auto areas = grip->GetNumberArray("targets/area", llvm::ArrayRef<double>());

	    for (auto area : areas) {
	        std::cout << "Got contour with area=" << area << std::endl;
	    }

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
