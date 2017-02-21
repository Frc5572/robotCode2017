#include "WPILib.h"
#include <drivetrain.h>
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
public:
	Robot() :
			driver(2), operat(3) {
		drivetrain::setMotors<DRIVETRAIN_MOTOR_TYPE>( {DRIVETRAIN_MOTOR_LEFT_ID}, {DRIVETRAIN_MOTOR_RIGHT_ID});
		drivetrain::init();
		intake::init();
		gear::init();
		shooter::init();
		climber::init();
		camera::init();
		shadow::init();
	}
	void RobotInit() override {
	}
	void OperatorControl() override {
		drivetrain::drop_versa();
		bool m = false;
		while (IsOperatorControl() && IsEnabled()) {
			shadow::dtwrite(-driver.L().second, -driver.R().second, .5 + (driver.LT() * .5));
			shadow::gwrite(operat.LT() > .1);
			shadow::swrite(operat.RT() > .1);

			//Versa
			if(driver.A())
				drivetrain::drop_versa();
			else if(driver.B())
				drivetrain::retract_versa();

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

			//Camera
			if(driver.Y() && !m){
				camera::switchc();
				m = true;
			}
			if(!driver.Y())
				m = false;

			Wait(0.005); // Motor timeout
		}
	}
	void Autonomous() {
		drivetrain::retract_versa();
		//shadow::run(this);
		autonomous::auto1(this);
	}
	void Test() override {
		shadow::start();
		while(IsTest() && IsEnabled()){
			shadow::dtwrite(-driver.L().second, -driver.R().second, 0.30);
			shadow::gwrite(driver.Y());
			shadow::swrite(driver.X());
		}
		shadow::stop();
	}
	void Disabled() override {
	}
};

START_ROBOT_CLASS(Robot)
