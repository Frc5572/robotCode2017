#include "gear.h"
#include "ports.h"
#include <DoubleSolenoid.h>
#include <WPILib.h>

frc::DoubleSolenoid *g;
frc::Compressor *c;

void gear::close(){
	g->Set(DoubleSolenoid::kForward);
}
void gear::open(){
	g->Set(DoubleSolenoid::kReverse);
}
void gear::stop(){
	g->Set(DoubleSolenoid::kOff);
}

void gear::init(){
	c = new Compressor(12);
	g = new DoubleSolenoid(12, GEAR_DOUBLE_SOLENOID_ID);
}
