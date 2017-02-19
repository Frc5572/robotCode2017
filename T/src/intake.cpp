#include "intake.h"
#include <ports.h>
#include <iostream>

INTAKE_MOTOR_TYPE *m_intake;

void intake::init() {
	m_intake = INTAKE_MOTOR;
}

void intake::intake(double m) {
	m_intake->Set(m);
}

void intake::stop() {
	m_intake->Set(0.0);
}
