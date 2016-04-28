#include <WPILib.h>
#include "UserInterface.h"
#include <stdio.h>
#include <string>
#include "DriveTrain/TankDrive.h"
#include "Shooter.h"
#include "GoalVision.h"
#include "Leddar.h"
#include "Autonomous.h"
#include "DriveTrain/SuspensionDrive.h"
//The Robot's name is "Sir. Wedgemore"
enum ShooterMode {
	STOW_MODE,
	PICKUP_MODE,
	BATTER_HIGOAL_MODE,
	DEFENSE_HIGOAL_MODE,
	MID_SHOT_MODE
};

class Wedgemore: public IterativeRobot

{
private:
	TankDrive m_tank;
	SuspensionDrive m_suspension;
	Shooter m_shooter;
	//Leddar m_leddar;
	UserInterface ui;
	WedgemoreUserInput wui;
	GoalVision m_goalVision;
	Autonomous m_auto;
	ADXRS450_Gyro m_rateSensor;
	ShooterMode m_shooterMode = STOW_MODE;
	VictorSP m_climber;
public:
	Wedgemore() :
		m_auto(&m_tank, &m_suspension, &m_shooter, &m_rateSensor, &m_goalVision),
		m_climber(0)
	{
	}

	void RobotInit()
	{

		//CameraServer::GetInstance()->StartAutomaticCapture("cam0");
		SmartDashboard::PutNumber("Driver P", 1);
		SmartDashboard::PutNumber("Driver I", 0);

		SmartDashboard::PutNumber("lift accel",		1);
		SmartDashboard::PutNumber("lift decel",		1);
		SmartDashboard::PutNumber("lift max speed",	0.5);

		SmartDashboard::PutNumber("Shooter P",		4);
		SmartDashboard::PutNumber("Shooter I",		0);
		SmartDashboard::PutNumber("Shooter D",		0);

		SmartDashboard::PutNumber("Wheel Comp", 1);
		SmartDashboard::PutBoolean("Save Prefs", false);

		SmartDashboard::PutNumber("Auto Mode Select", 0);
		SmartDashboard::PutNumber("Total Distance", 260);
		SmartDashboard::PutBoolean("Calibrate", false);
		SmartDashboard::PutBoolean("Save Captured Image", false);
		//Create keys if they do not exist already.
		if(!Preferences::GetInstance()->ContainsKey("Auto left Goal x1")) {
			Preferences::GetInstance()->PutFloat("Auto left Goal x1", 150);
			Preferences::GetInstance()->PutFloat("Auto left Goal x2", 300);
			Preferences::GetInstance()->PutFloat("Auto left Goal y1", 110);
			Preferences::GetInstance()->PutFloat("Auto left Goal y2", 110);
		}
		//m_goalVision.Init();
		m_tank.Init();
		//m_leddar.StartAutoDetections(true);
//		Preferences::GetInstance()->PutFloat("Drive P", 1);
//		Preferences::GetInstance()->PutFloat("Drive I", 0);
//		Preferences::GetInstance()->PutFloat("Drive D", 0);
//		Preferences::GetInstance()->PutFloat("Lift P", 4);
//		Preferences::GetInstance()->PutFloat("Lift I", 0.01);
//		Preferences::GetInstance()->PutFloat("Lift D", 0);
	}

	void AutonomousInit()
	{
		m_shooter.Zero();
		int automode = SmartDashboard::GetNumber("Auto Mode Select", 0);
		m_auto.Init(automode);
		m_goalVision.Init();

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
		//m_leddar.StartAutoDetections(true);


	}

	void TeleopPeriodic()
	{

		//SmartDashboard::PutNumber("Angle", m_rateSensor.GetAngle());
		//SmartDashboard::PutNumber("Rate", m_rateSensor.GetRate());
		//m_leddar.FillBuffer();
		//SmartDashboard::PutNumber("Num Detections", m_leddar.GetDetections().size());
//		LineSeg lineSegs[16];
//		unsigned numSegs = m_leddar.GetDetectionsAsLineSegs(lineSegs, 16);
//		SmartDashboard::PutNumber("Num Linesegs", numSegs);
//		cout << "num segments: " << numSegs << endl;
//		for(unsigned i = 0; i < numSegs; i++) {
//			cout << "length of seg " << i <<": " << lineSegs[i].length << endl;
//		}
		ui.GetData(&wui);

		float leftSpeed = wui.LeftSpeed;
		float rightSpeed = wui.RightSpeed;
		if(wui.RunGunLight) {
			leftSpeed  *= 0.7;
			rightSpeed *= 0.7;
		}
		m_tank.Drive(leftSpeed, rightSpeed);

		if(wui.PickupPos || wui.ShooterDown) {
			m_shooterMode = PICKUP_MODE;
		}
		else if(wui.StartPosition) {	//start & stow pos
			m_shooterMode = STOW_MODE;
		}
		else if(wui.BatterHiGoal) {
			m_shooterMode = BATTER_HIGOAL_MODE;
		}
		else if(wui.DefenseHiGoal) {
			m_shooterMode = DEFENSE_HIGOAL_MODE;
		}
		else if(wui.MidHiGoal) {
			m_shooterMode = MID_SHOT_MODE;
		}
		if(wui.SpinUp) {
			if(m_shooterMode == BATTER_HIGOAL_MODE) {
				m_shooter.Spinup(2600);
			}
			else if(m_shooterMode == MID_SHOT_MODE) {
							m_shooter.Spinup(4500);
						}
			else {
				m_shooter.Spinup(5000);
			}

		}
		else if(wui.SpinUpLow) {
			m_shooter.Spinup(3500);
		}

		else if (!wui.Pickup){
			m_shooter.Stop();
		}
		if(wui.Shoot) {
			m_shooter.Shoot(wui.Shoot);
		}
		else {
			m_shooter.LimitShoot(wui.LimitShoot, m_rateSensor.GetRate());
		}
		if(wui.Pickup) {
			m_shooter.Pickup();
		}
		if(wui.Climber == 0){ //CLIMBER CODE HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			m_climber.Set(-1.0);
		}
		else {
			m_climber.Set(0.0);
		}

//		if(wui.Custom) {
//			m_shooter.LiftTo(SmartDashboard::GetNumber("ShooterAngle", 0));
//		}
		if(wui.Zero) {
			m_shooterMode = STOW_MODE;
			m_shooter.Zero();
			//m_shooter.shooter_zero = 1;
		}

		m_shooter.ToggleRunLight(wui.RunGunLight);

		if(wui.SpeedMode) {
			m_tank.SetMode(SPEED_MODE);
			SmartDashboard::PutBoolean("VBus Mode", false);
		}
		if(wui.VBusMode) {
			m_tank.SetMode(VBUS_MODE);
			SmartDashboard::PutBoolean("VBus Mode", true);
		}
		m_tank.Reverse(wui.ReverseDrive);
		float AngleAdjust = wui.LiftSpeed * ((-wui.SliderValue + 1)*1.5);//
		switch(m_shooterMode) {
			case STOW_MODE:
				m_shooter.LiftTo(AngleAdjust * 30);
				break;
			case PICKUP_MODE:
				m_shooter.LiftTo(180 - AngleAdjust * 35);//TODO use preferences for values.
				break;
			case BATTER_HIGOAL_MODE:
				m_shooter.LiftTo(64); //TODO use preferences for values.
				break;
			case DEFENSE_HIGOAL_MODE:
				m_shooter.LiftTo(34 + AngleAdjust*20);
				//m_shooter.LiftTo(36.6 + AngleAdjust*20); //TODO use preferences for values.
				break;
			case MID_SHOT_MODE:
				m_shooter.LiftTo(37 + AngleAdjust*20); //TODO use preferences for values.
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
		m_shooter.Shoot(false);
		m_shooter.Spinup(0);
		m_tank.Init();


		//m_leddar.StartAutoDetections(false);

	}

	void DisabledPeriodic() override {
		static unsigned count = 0;
		if(count % 100 == 0) {
			if(SmartDashboard::GetBoolean("Calibrate", false)) {
				m_rateSensor.Calibrate();
				SmartDashboard::PutBoolean("Calibrate", false);
			}
			SmartDashboard::PutNumber("RS Angle", m_rateSensor.GetAngle());
		}

		bool setPref = SmartDashboard::GetBoolean("Save Prefs", false);
		if (true == setPref) {
			float wheelComp = SmartDashboard::GetNumber("Wheel Comp", 1);
			Preferences::GetInstance()->PutFloat("Wheel Comp Pref", wheelComp);

			SmartDashboard::PutBoolean("Save Prefs", false);
		}
//		Point start, end;

//		start.x = Preferences::GetInstance()->GetInt("Auto left Goal x1", 0);
//		start.y = Preferences::GetInstance()->GetInt("Auto left Goal y1", 0);
//		end.x = Preferences::GetInstance()->GetInt("Auto left Goal x2", 0);
//		end.y = Preferences::GetInstance()->GetInt("Auto left Goal y2", 0);

//		m_goalVision.SetLine(start, end);
//		if(m_goalVision.GetAngleCorrection()) {
//			SmartDashboard::PutBoolean("target found", true);
//		} else {
//			SmartDashboard::PutBoolean("target found", false);
//		}
		count++;
	}
};

START_ROBOT_CLASS(Wedgemore)
