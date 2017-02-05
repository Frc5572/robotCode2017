#include "controller.h"
#include <WPILib.h>
#include <Joystick.h>

#define LEFT_z 2
#define LEFT_x 0
#define LEFT_y 1

#define RIGHT_z 3
#define RIGHT_x 4
#define RIGHT_y 5

#define LEFT_BUTTON 5
#define RIGHT_BUTTON 6

#define X_BUTTON 3
#define Y_BUTTON 4
#define A_BUTTON 1
#define B_BUTTON 2

#define START_BUTTON 8
#define BACK_BUTTON 7

#define LEFT_STICK_BUTTON 9
#define RIGHT_STICK_BUTTON 10

FRC5572Controller::FRC5572Controller(int I) {
	pad = new XboxController(I);
}

FRC5572Controller::~FRC5572Controller() {
	delete pad;
}

double FRC5572Controller::LT() {
	return pad->GetRawAxis(LEFT_z);
}

bool FRC5572Controller::LB() {

	return pad->GetRawButton(LEFT_BUTTON);
}

double FRC5572Controller::RT() {
	return pad->GetRawAxis(RIGHT_z);
}

bool FRC5572Controller::RB() {

	return pad->GetRawButton(RIGHT_BUTTON);
}

bool FRC5572Controller::X() {

	return pad->GetRawButton(X_BUTTON);
}

bool FRC5572Controller::Y() {

	return pad->GetRawButton(Y_BUTTON);
}

bool FRC5572Controller::A() {

	return pad->GetRawButton(A_BUTTON);
}

bool FRC5572Controller::B() {

	return pad->GetRawButton(B_BUTTON);
}

std::pair<double, double> FRC5572Controller::L() {

	return std::make_pair(pad->GetRawAxis(LEFT_x), pad->GetRawAxis(LEFT_y));
}

std::pair<double, double> FRC5572Controller::R() {

	return std::make_pair(pad->GetRawAxis(RIGHT_x), pad->GetRawAxis(RIGHT_y));
}

FRC5572Controller::pov FRC5572Controller::POV() {
	switch (pad->GetPOV(0)){
	case 0 : return NORTH;
	case 1 : return NORTHEAST;
	case 2 : return EAST;
	case 3 : return SOUTHEAST;
	case 4 : return SOUTH;
	case 5 : return SOUTHWEST;
	case 6 : return WEST;
	case 7 : return NORTHWEST;
	default : return NONE;
	}
}

bool FRC5572Controller::start() {
	return pad->GetRawButton(START_BUTTON);
}

bool FRC5572Controller::back() {
	return pad->GetRawButton(BACK_BUTTON);
}

bool FRC5572Controller::Lbutton() {
	return pad->GetRawButton(LEFT_STICK_BUTTON);
}

bool FRC5572Controller::Rbutton() {
	return pad->GetRawButton(RIGHT_STICK_BUTTON);
}

void FRC5572Controller::rumble(double x, double y) {
	pad->SetRumble(GenericHID::kLeftRumble, x);
	pad->SetRumble(GenericHID::kRightRumble, y);
}
