#include "WPILib.h"
#include <drivetrain.h>
//#include <SmartDashboard/SendableChooser.h>
#include <shooter.h>
#include <climber.h>
#include <camera.h>
#include <gear.h>
#include <string>
#include "controller.h"
#include <iostream>
#include <ports.h>
#include <intake.h>
#include <shadow.h>

class Robot: public SampleRobot {
	FRC5572Controller driver;
	FRC5572Controller operat;
	//frc::SendableChooser<unsigned> *sc;
public:
	Robot() :
			driver(2), operat(3) {
		drivetrain::setMotors<DRIVETRAIN_MOTOR_TYPE>( {DRIVETRAIN_MOTOR_LEFT_ID}, {DRIVETRAIN_MOTOR_RIGHT_ID});
		drivetrain::init();
		intake::init();
		gear::init();
		shooter::init();
		climber::init();
		//camera::init();
		shadow::init();
		//sc = new SendableChooser<unsigned>();
	}
	void RobotInit() override {
		/*
		sc->AddDefault("Left", 0);
		sc->AddObject("Middle", 1);
		sc->AddObject("Right", 2);
		sc->AddObject("Left Boiler", 3);
		sc->AddObject("Right Boiler", 4);
		sc->AddObject("Middle Boiler", 5);
		sc->AddObject("Get outta da way", 6);
		sc->AddObject("Get Nuthin", 6);
		SmartDashboard::PutData("Autonomous Mode", sc);
		*/
	}
	void OperatorControl() override {
		drivetrain::drop_versa();
		while (IsOperatorControl() && IsEnabled()) {
			shadow::dtwrite(-driver.L().second, -driver.R().second, .5 + (driver.LT() * .5));
			shadow::gwrite(operat.LT() > .1);
			shadow::swrite(operat.RT() > .1);

			//Versa
			if(driver.A())
				shadow::vwrite(false);
			else if(driver.B())
				shadow::vwrite(true);

			//Climber
			climber::climb(driver.RT());
			if(driver.RB())
				climber::prime();
			else
				climber::reset();

			// Intake
			if(operat.A())
				intake::intake(.7);
			else
				intake::intake(0);
			/*
			//Camera
			if(driver.Y() && !m){
				camera::switchc();
				m = true;
			}
			if(!driver.Y())
				m = false;

			Wait(0.005); // Motor timeout
			*/
		}
	}
	void Autonomous() {
		//unsigned mode = sc->GetSelected();
		//std::cout << mode << std::endl;
		Timer t;
		t.Start();
		drivetrain::retract_versa();
		//shadow::run(this);
		autonomous::auto1(this);
		autonomous::auto2(this);
		autonomous::auto3(this);
		autonomous::disengage();
		autonomous::auto5(this);
		while(IsAutonomous() && IsEnabled()){
			shooter::shoot();
		}
		shooter::stop();
		drivetrain::drive_lr(0, 0, -1);
		t.Stop();
		std::cout << t.Get() << std::endl;
	}
	void Test() override {
		shadow::start();
		while(IsTest() && IsEnabled()){
			shadow::dtwrite(-driver.L().second, -driver.R().second, .5 + (driver.LT() * .5));
			shadow::gwrite(driver.Y());
			shadow::swrite(driver.X());
		}
		shadow::stop();
	}
	void Disabled() override {
	}
};

START_ROBOT_CLASS(Robot)
