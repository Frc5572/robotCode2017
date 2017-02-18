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
		drivetrain::retract_versa();
		while (IsOperatorControl() && IsEnabled()) {
			shadow::dtwrite(-driver.L().second, -driver.R().second, 0.30);
			if(driver.A()) {
				drivetrain::drop_versa();
			} else if (driver.B()) {
				drivetrain::retract_versa();
			}
			shadow::gwrite(driver.Y());
			if(driver.RB())
				intake::intake();
			else
				intake::stop();
			shadow::swrite(driver.X());
			if(driver.LB())
				climber::climb(driver.LT());
			else
				climber::climb(0);
			Wait(0.005);
			if(operat.A() && camera::version()){
				camera::switchc();
			} else if(operat.B() && !camera::version()){
				camera::switchc();
			}
			//std::cout << camera::version() << std::endl;
		}
	}
	void Autonomous() {
		shadow::run(this);
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
