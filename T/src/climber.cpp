#include "climber.h"
#include "ports.h"

frc::DoubleSolenoid *primer;
CLIMBER_MOTOR_TYPE *climbe;

void climber::init() {
	primer = new frc::DoubleSolenoid(12, CLIMBER_DOUBLE_SOLENOID_ID);
	climbe = CLIMBER_MOTOR;
	primer->Set(frc::DoubleSolenoid::kForward);
}

void climber::prime() {
	primer->Set(frc::DoubleSolenoid::kReverse);
}

void climber::reset() {
	primer->Set(frc::DoubleSolenoid::kForward);
}

void climber::climb(double a) {
	climbe->Set(a);
}
