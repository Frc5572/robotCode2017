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
		//climber::init();
		//camera::init();
	}
	void RobotInit() override {
	}
	void OperatorControl() override {
		drivetrain::retract_versa();
		while (IsOperatorControl() && IsEnabled()) {
			drivetrain::drive_lr(-driver.L().second, -driver.R().second, 0.30);
			if(driver.A()) {
				drivetrain::drop_versa();
			} else if (driver.B()) {
				drivetrain::retract_versa();
			}
			if(driver.Y()) {
				gear::open();
			} else {
				gear::close();
			}
			bool stop = true;
			if(driver.LB()){
				intake::intake();
				stop = false;
			}
			if(driver.X()){
				shooter::shoot();
				stop = false;
			}else{
				shooter::stop();
			}
			if(stop)
				intake::stop();
			//if(driver.X())
			//	climber::climb(driver.LT());
			//else
			//	climber::climb(0);
			//Wait(0.005);
			//if(operat.A() && camera::version()){
			//	camera::switchc();
			//} else if(operat.B() && !camera::version()){
			//	camera::switchc();
			//}
			//std::cout << camera::version() << std::endl;
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
