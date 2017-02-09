#include "gear.h"
#include "ports.h"
#include <DoubleSolenoid.h>
#include <WPILib.h>

frc::DoubleSolenoid *g;

void gear::close(){
	g->Set(DoubleSolenoid::kReverse);
}
void gear::open(){
	g->Set(DoubleSolenoid::kForward);
}
void gear::stop(){
	g->Set(DoubleSolenoid::kOff);
}

void gear::init(){
	g = new DoubleSolenoid(12, GEAR_DOUBLE_SOLENOID_ID);
}
