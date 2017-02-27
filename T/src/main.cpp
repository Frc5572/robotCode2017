#include "WPILib.h"
#include <drivetrain.h>
#include <SmartDashboard/SendableChooser.h>
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
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>


class Robot: public SampleRobot {
	FRC5572Controller driver;
	FRC5572Controller operat;
	//frc::SendableChooser<int> *sc;
public:
	Robot() :
		driver(2), operat(3) {
		//sc=new SendableChooser<int>();
		wire = new I2C(I2C::kMXP, ARDUINO_ID);
		drivetrain::setMotors<DRIVETRAIN_MOTOR_TYPE>( {DRIVETRAIN_MOTOR_LEFT_ID}, {DRIVETRAIN_MOTOR_RIGHT_ID}); //drive motor initialization
		drivetrain::init();//Drivetrain initialization
		intake::init();//Ball Intake initialization
		gear::init();//Gear initialization
		shooter::init();//Shooter initialization
		climber::init();
		SmartDashboard::PutNumber("Auto", 0);
		//shadow::init();
	}

	void OperatorControl() override {
		drivetrain::drop_versa();
		while (IsOperatorControl() && IsEnabled()) {
			shadow::dtwrite(-driver.L().second, -driver.R().second, .75 + (driver.LT() * .25));
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

		}
	}
	void Autonomous() override {
		//unsigned mode = sc->GetSelected();
		//std::cout << mode << std::endl;
		Timer t;
		t.Start();
		drivetrain::retract_versa();
			switch((int)SmartDashboard::GetNumber("Auto",0)){
			case 1: //.. Auto 1
				autonomous::auto1(this);
				break;
			case 2: //.. Auto 2
				autonomous::auto2(this);
				break;
			//default:
				//exit -1;
			}

		//shadow::run(this);
		std::cout << "" << std::endl;
		//autonomous::auto1(this);
		//autonomous::auto2(this);
		//autonomous::auto3(this);
		//autonomous::disengage();
		//autonomous::auto5(this);
		//while(IsAutonomous() && IsEnabled()){
			//shooter::shoot();
		//}
		//shooter::stop();
		drivetrain::drive_lr(0, 0, -1);
		t.Stop();
		//std::cout << t.Get() << std::endl;
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
