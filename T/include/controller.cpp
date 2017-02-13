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

bool k = false;

void ct(XboxController *pad, FRC5572Controller::ButtonEvent x,
		FRC5572Controller::ButtonEvent y, FRC5572Controller::ButtonEvent lb,
		FRC5572Controller::ButtonEvent rb, FRC5572Controller::ButtonEvent a,
		FRC5572Controller::ButtonEvent b) {
	if (pad->GetRawButton(X_BUTTON)) {
		std::thread m(x);
		m.detach();
	}
	if (pad->GetRawButton(Y_BUTTON)) {
		std::thread m(y);
		m.detach();
	}
	if (pad->GetRawButton(A_BUTTON)) {
		std::thread m(a);
		m.detach();
	}
	if (pad->GetRawButton(B_BUTTON)) {
		std::thread m(b);
		m.detach();
	}
	if (pad->GetRawButton(LEFT_BUTTON)) {
		std::thread m(lb);
		m.detach();
	}
	if (pad->GetRawButton(RIGHT_BUTTON)) {
		std::thread m(rb);
		m.detach();
	}
}

FRC5572Controller::FRC5572Controller(int I) {
	pad = new XboxController(I);
}

FRC5572Controller::~FRC5572Controller() {
	delete pad;
	if (!k) {
		k = true;
		s.join();
	}
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

unsigned FRC5572Controller::POV() {
	return pad->GetPOV(0) / 45;
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

void FRC5572Controller::runEvents() {
	ct(pad, x, y, lb, rb, a, b);
}
