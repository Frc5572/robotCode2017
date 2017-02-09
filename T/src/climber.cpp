#include "climber.h"
#include "ports.h"

frc::DoubleSolenoid *primer;
CLIMBER_MOTOR_TYPE *climbe;

void climber::init() {
	primer = new frc::DoubleSolenoid(12, CLIMBER_DOUBLE_SOLENOID);
	climbe = CLIMBER_MOTOR;
}

void climber::prime() {
	primer->Set(frc::DoubleSolenoid::kForward);
}

void climber::reset() {
	primer->Set(frc::DoubleSolenoid::kReverse);
}

void climber::climb(double a) {

}
