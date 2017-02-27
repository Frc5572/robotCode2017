#include "shooter.h"
#include "ports.h"
#include "gear.h"
#include <cmath>
#include <Timer.h>
#include <drivetrain.h>
#include <gear.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>


AGITATOR_MOTOR_TYPE*m_agitator;


union {
	float Double;
	uint8_t Byte[4];
}Recieve_DataS;

union {
	float Double;
	uint8_t Byte[4];
}Recieve_DataR;

union {
	float Double;
	uint8_t Byte[4];
}Recieve_DataL;

SHOOTER_MOTOR_TYPE *m_launcher;
SHOOTER_PRIME_MOTOR_TYPE *m_primer;

char message[2];

long Shooter_RPM = 0;
unsigned char transmit_data[ARDUINO_TRAN_DATA_SIZE];
bool sentRPM = false;


void SendShootRPM() {
	//Shooter_RPM = distance.distance;
	std::cout << "Sending rpms" << std::endl;
	Shooter_RPM = SmartDashboard::GetNumber("rpms", 0);
	transmit_data[0] = 'S';
	transmit_data[1] = 'H';
	//wire->WriteBulk(transmit_data, ARDUINO_TRAN_DATA_SIZE);
	transmit_data[2] = Shooter_RPM >> 8;
	transmit_data[3] = Shooter_RPM;
	std::cout << "Sending rpms" << transmit_data << std::endl;
	wire->WriteBulk(transmit_data, ARDUINO_TRAN_DATA_SIZE);
}

void StopShootRPMCycle() {
	transmit_data[0] = 'S';
	transmit_data[1] = 'T';
	transmit_data[2] = 0;
	transmit_data[3] = 0;
	wire->WriteBulk(transmit_data, ARDUINO_TRAN_DATA_SIZE);
}

void Start(char a, char b, char c = 0, char d = 0) {
	transmit_data[0] = a;
	transmit_data[1] = b;
	transmit_data[2] = c;
	transmit_data[3] = d;
	wire->WriteBulk(transmit_data, ARDUINO_TRAN_DATA_SIZE);
}

void ReadI2C() {
	uint8_t data[ARDUINO_REC_DATA_SIZE];
	for (unsigned i = 0; i < ARDUINO_REC_DATA_SIZE; i++)
		data[i] = 0;
	wire->ReadOnly(ARDUINO_REC_DATA_SIZE, data);

	message[0] = data[0];
	message[1] = data[1];

	for (int i = 0; i < 4; i++)
		Recieve_DataR.Byte[i] = data[i + 2];
	for (int i = 0; i < 4; i++)
		Recieve_DataL.Byte[i] = data[i + 6];
	for (int i = 0; i < 4; i++)
		Recieve_DataS.Byte[i] = data[i + 10];

	SmartDashboard::PutString("Code", message);
	SmartDashboard::PutNumber("Time", Timer::GetMatchTime());
	usleep(1000);

}

void shooter::init() {
	m_launcher = SHOOTER_MOTOR;
	m_primer = SHOOTER_PRIME_MOTOR; //Intake motor
	m_agitator = AGITATOR_MOTOR;
	wire = new I2C(I2C::kMXP, ARDUINO_ID);
	Recieve_DataS.Double = 0;
	if (SmartDashboard::GetNumber("rpms", -1) == -1)
		SmartDashboard::PutNumber("rpms", 13000.0);
}

void shooter::shoot() {
	if (!sentRPM) {
		//delete wire;
		//wire = new I2C(I2C::kMXP, ARDUINO_ID);
		SendShootRPM();
		sentRPM = true;
		message[0] = 'R';
	}

	ReadI2C();
	m_launcher->Set(-Recieve_DataS.Double);
	SmartDashboard::PutNumber("power", Recieve_DataS.Double);
	if (message[0] == 'I' && message[1] == 'N') {
		m_agitator->Set(frc::Relay::kOn);
		m_primer->Set(INDEXER_POWER);
	} else {
		m_primer->Set(0.0);
		m_agitator->Set(frc::Relay::kOff);
	}

}

void shooter::stop() {
	if (sentRPM) {
		StopShootRPMCycle();
		sentRPM = false;
	}
	m_launcher->Set(0);
	m_primer->Set(0);
	m_agitator->Set(frc::Relay::kOff);
}

void autonomous::auto1(RobotBase *rb) { // Go straight only on middle gear peg release and drive back
	StopShootRPMCycle();
	gear::close();
	DriveStraight(rb, '1');
	gear::open();
	Wait(WAIT_DROPOFF_GEAR);
	DriveBack();
}

void autonomous::auto2(RobotBase *rb) { // Left Side auto. Go straight, turn right, go straight, release gear, drive back
	StopShootRPMCycle();
	gear::close();
	DriveStraight(rb, '2');
	Rotate(rb, '3');
	DriveStraight(rb, '4');
	gear::open();
	Wait(WAIT_DROPOFF_GEAR);
	DriveBack();
}

void autonomous::auto3(RobotBase *rb) { // Left Side auto. Go straight, turn right, go straight, release gear, drive back
	StopShootRPMCycle();
	gear::close();
	DriveStraight(rb, '2');
	Rotate(rb, '3');
	DriveStraightRotateShoot(rb, '7');
	gear::open();
	Wait(WAIT_DROPOFF_GEAR);
	DriveBack();
}


void autonomous::DriveStraightRotateShoot(RobotBase *rb, char mode){
	while ((message[0] != 'O' || message[1] != 'O') && rb->IsEnabled()
				&& rb->IsAutonomous())
			ReadI2C(); // Wait until we get OO
		Start('A', mode); // Send A 'mode'
		while (rb->IsEnabled() && rb->IsAutonomous()) {
			ReadI2C(); // Get Drivetrain Power values
			m_launcher->Set(-Recieve_DataS.Double);
			SmartDashboard::PutNumber("LeftPower", Recieve_DataL.Double);
			SmartDashboard::PutNumber("RightPower", Recieve_DataR.Double);
			SmartDashboard::PutString("Code", message);

			if (message[0] == 'D' && message[1] == 'R'){ // Driving forward
				drivetrain::drive_lr((double) Recieve_DataL.Double,
										(double) Recieve_DataR.Double, -1.0); // Drive with raw values
			}

			if (message[0] == 'T' && message[1] == 'R'){ // If reached target
				gear::open();
				Wait(WAIT_DROPOFF_GEAR);
				Start('G','D'); //Gear Dropped
			}

			if (message[0] == 'B' && message[1] == 'A'){ // If reached target
				Start('N','R'); //Now Rotate
			}

			if (message[0] == 'G' && message[1] == 'Y'){ // Driving forward
							drivetrain::drive_lr((double) Recieve_DataL.Double,
													(double) Recieve_DataR.Double, -1.0); // Drive with raw values
						}

			if (message[0] == 'R' && message[1] == 'R') { // If received encoder values
				break;

		}
		//std::cout << "finished" << std::endl;
		StopShootRPMCycle(); // Send ST
		drivetrain::drive_lr(0, 0, -1); //Stop moving

}



void autonomous::DriveStraight(RobotBase *rb, char mode){
	while ((message[0] != 'O' || message[1] != 'O') && rb->IsEnabled()
				&& rb->IsAutonomous())
			ReadI2C(); // Wait until we get OO
		Start('A', mode); // Send A 'mode'
		while (rb->IsEnabled() && rb->IsAutonomous()) {
			ReadI2C(); // Get Drivetrain Power values
			SmartDashboard::PutNumber("LeftPower", Recieve_DataL.Double);
			SmartDashboard::PutNumber("RightPower", Recieve_DataR.Double);
			SmartDashboard::PutString("Code", message);
			if (message[0] == 'T' && message[1] == 'R') // If reached target
				break;
			if (message[0] == 'E' && message[1] == 'N') { // If received encoder values
				drivetrain::drive_lr((double) Recieve_DataL.Double,
						(double) Recieve_DataR.Double, -1.0); // Drive with raw values
			} else {
				drivetrain::drive_lr(0, 0, -1); // Don't do anything
			}
		}
		//std::cout << "finished" << std::endl;
		StopShootRPMCycle(); // Send ST
		drivetrain::drive_lr(0, 0, -1); //Stop moving

}


void autonomous::Rotate(RobotBase *rb, char mode) {
	StopShootRPMCycle();
	drivetrain::drop_versa();
	while ((message[0] != 'O' || message[1] != 'O') && rb->IsEnabled()
			&& rb->IsAutonomous())
		ReadI2C();
	Start('A', mode);
	while (rb->IsEnabled() && rb->IsAutonomous()) {
		ReadI2C();
		SmartDashboard::PutNumber("LeftPower", Recieve_DataL.Double);
		SmartDashboard::PutNumber("RightPower", Recieve_DataR.Double);
		SmartDashboard::PutString("Code", message);
		if (message[0] == 'R' && message[1] == 'R')
			break;
		if (message[0] == 'G' && message[1] == 'Y') {
			drivetrain::drive_lr((double) Recieve_DataL.Double,
					(double) Recieve_DataR.Double, -1.0);
		} else {
			drivetrain::drive_lr(0, 0, -1);
		}
	}
	std::cout << "finished" << std::endl;
	StopShootRPMCycle();
	drivetrain::drive_lr((double) 0, (double) 0, -1.0);
}

void autonomous::auto3(RobotBase* rb) {
	drivetrain::retract_versa();
	StopShootRPMCycle();
	while ((message[0] != 'O' || message[1] != 'O') && rb->IsEnabled()
			&& rb->IsAutonomous())
		ReadI2C();
	gear::close();
	Start('A', '3');
	while (rb->IsEnabled() && rb->IsAutonomous()) {
		ReadI2C();
		SmartDashboard::PutNumber("LeftPower", Recieve_DataL.Double);
		SmartDashboard::PutNumber("RightPower", Recieve_DataR.Double);
		SmartDashboard::PutString("Code", message);
		if (message[0] == 'T' && message[1] == 'R')
			break;
		if (message[0] == 'E' && message[1] == 'N') {
			drivetrain::drive_lr((double) Recieve_DataL.Double,
					(double) Recieve_DataR.Double, -1.0);
		} else {
			drivetrain::drive_lr(0, 0, -1);
		}
	}
	std::cout << "finished" << std::endl;
	StopShootRPMCycle();
	drivetrain::drive_lr(0, 0, -1);
}

void autonomous::auto4(RobotBase *rb) {
	StopShootRPMCycle();
	drivetrain::drop_versa();
	while ((message[0] != 'O' || message[1] != 'O') && rb->IsEnabled()
			&& rb->IsAutonomous())
		ReadI2C();
	Start('A', '4');
	while (rb->IsEnabled() && rb->IsAutonomous()) {
		ReadI2C();
		SmartDashboard::PutNumber("LeftPower", Recieve_DataL.Double);
		SmartDashboard::PutNumber("RightPower", Recieve_DataR.Double);
		SmartDashboard::PutString("Code", message);
		if (message[0] == 'R' && message[1] == 'R')
			break;
		if (message[0] == 'G' && message[1] == 'Y') {
			drivetrain::drive_lr((double) Recieve_DataL.Double,
					(double) Recieve_DataR.Double, -1.0);
		} else {
			drivetrain::drive_lr(0, 0, -1);
		}
	}
	std::cout << "finished" << std::endl;
	StopShootRPMCycle();
	drivetrain::drive_lr((double) 0, (double) 0, -1.0);
}

void autonomous::auto5(RobotBase *rb) {
	StopShootRPMCycle();
	drivetrain::drop_versa();
	while ((message[0] != 'O' || message[1] != 'O') && rb->IsEnabled()
			&& rb->IsAutonomous())
		ReadI2C();
	Start('A', '5');
	while (rb->IsEnabled() && rb->IsAutonomous()) {
		ReadI2C();
		SmartDashboard::PutNumber("LeftPower", Recieve_DataL.Double);
		SmartDashboard::PutNumber("RightPower", Recieve_DataR.Double);
		SmartDashboard::PutString("Code", message);
		if (message[0] == 'R' && message[1] == 'R')
			break;
		if (message[0] == 'G' && message[1] == 'Y') {
			drivetrain::drive_lr((double) Recieve_DataL.Double,
					(double) Recieve_DataR.Double, -1.0);
		} else {
			drivetrain::drive_lr(0, 0, -1);
		}
	}
	std::cout << "finished" << std::endl;
	StopShootRPMCycle();
	drivetrain::drive_lr((double) 0, (double) 0, -1.0);
}

void autonomous::DriveBack(){
	drivetrain::drive_lr(0.4, 0.4, -1);
	Wait(0.1);
	drivetrain::drive_lr(0.5, 0.5, -1);
	Wait(0.1);
	drivetrain::drive_lr(0.6, 0.6, -1);
	Wait(0.1);
	drivetrain::drive_lr(0.8, 0.8, -1);
	Wait(0.6);
	drivetrain::drive_lr(0, 0, -1);
}
