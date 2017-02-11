#include "intake.h"
#include <ports.h>
#include <iostream>

AGITATOR_MOTOR_TYPE *m_agitator;
INTAKE_MOTOR_TYPE *m_intake;

void intake::init() {
	m_agitator = AGITATOR_MOTOR;
	m_intake = INTAKE_MOTOR;
}

void intake::intake() {
	m_intake->Set(1.0);
}

void intake::stop() {
	std::cout << "a" << std::endl;
	m_intake->Set(0.0);
	std::cout << "b" << std::endl;
	//m_agitator->Set(frc::Relay::kOff);
}

void intake::agitate() {
	m_agitator->Set(frc::Relay::kOn);
}
