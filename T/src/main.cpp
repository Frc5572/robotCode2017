#include "WPILib.h"
#include <drivetrain.h>
#include <shooter.h>
#include <climber.h>
#include <camera.h>
#include <gear.h>
#include <string>
#include <controller.h>
#include <iostream>

class Robot: public SampleRobot {
	FRC5572Controller driver;
	FRC5572Controller operat;

	//Pnuematics
public:
	Robot() :
			driver(2), operat(3) {
		drivetrain::setMotors<VictorSP>( { 0 }, { 3 });
		//shooter::init();
		//climber::init();
	}
	void RobotInit() override {
	}
	void OperatorControl() override {
		while (IsOperatorControl() && IsEnabled()) {
			drivetrain::drive_tank(-driver.L().first, -driver.L().second, 0.5);
			if (driver.A())
				drivetrain::drop_versa();
			else if (driver.B())
				drivetrain::retract_versa();

			/*
			if(operat.A())
				shooter::shoot();
			else
				shooter::stop();
			//*/

			/*
			if(driver.X())
				climber::climb(driver.LT());
			else
				climber::climb(0);
			//*/
			Wait(0.005);
		}
	}
	void Autonomous() {
	}
	void Test() override {
	}
	void Disabled() override {
	}
};

START_ROBOT_CLASS(Robot)
