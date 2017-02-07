/*
 * pnuematics.cpp
 *
 *  Created on: Feb 5, 2017
 *      Author: stem
 */
#include <DoubleSolenoid.h>
#include <WPILib.h>
#include <Compressor.h>
#include "pnuematics.h"
#define LIFT1_DOUBLE_SOLENOID 0,7
#define LIFT2_DOUBLE_SOLENOID 1,6
#define GEAR_DOUBLE_SOLENOID  2,5
frc::Compressor *comp;
frc::DoubleSolenoid *lift1;
frc::DoubleSolenoid *primer_climber;
frc::DoubleSolenoid *gear;

void p::closeGear(){
	gear->Set(DoubleSolenoid::kReverse);
}
void p::openGear(){
	gear->Set(DoubleSolenoid::kForward);
}
void p::stopGear(){
	gear->Set(DoubleSolenoid::kOff);
}
void p::liftDown(){
	lift1->Set(DoubleSolenoid::kReverse);
}
void p::stopLift(){
	lift1->Set(DoubleSolenoid::kReverse);
}
void p::liftUp(){
	lift1->Set(DoubleSolenoid::kForward);
}
void p::init(){
	comp = new Compressor(12);
	lift1 = new DoubleSolenoid(12, LIFT1_DOUBLE_SOLENOID);
	primer_climber = new DoubleSolenoid(12, LIFT2_DOUBLE_SOLENOID);
	gear = new DoubleSolenoid(12, GEAR_DOUBLE_SOLENOID);
}

void p::prime() {
	primer_climber->Set(DoubleSolenoid::kForward);
}

void p::latch() {
	primer_climber->Set(DoubleSolenoid::kReverse);
}

frc::Compressor* p::getCompressor() {
	return comp;
}

frc::DoubleSolenoid* p::getVersa() {
	return lift1;
}

frc::DoubleSolenoid* p::getPrimer() {
	return primer_climber;
}

frc::DoubleSolenoid* p::getGear() {
	return gear;
}
