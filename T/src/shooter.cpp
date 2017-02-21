#include "shooter.h"
#include <I2C.h>
#include <iostream>
#include <enet.h>
#include "ports.h"
#include <DynamicLookupTable.h>
#include <cmath>
#include <Timer.h>
#include <drivetrain.h>
#include <gear.h>

#define TWOVAR

union S {
	enet_uint8 byte[sizeof(long double)];
	long double distance;
};

volatile S distance;
AGITATOR_MOTOR_TYPE*m_agitator;

#ifdef TWOVAR
union {
	float Double;
	uint8_t Byte[ARDUINO_REC_DATA_SIZE];
} Power;
#endif

union {
	float Double;
	uint8_t Byte[ARDUINO_REC_DATA_SIZE];
} Recieve_DataR;

union {
	float Double;
	uint8_t Byte[ARDUINO_REC_DATA_SIZE];
} Recieve_DataL;

SHOOTER_MOTOR_TYPE *m_launcher;
SHOOTER_PRIME_MOTOR_TYPE *m_primer;
I2C* wire;
char code[2];

#ifdef TWOVAR
char code2[2];
#endif

long Shooter_RPM = 0;
unsigned char transmit_data[ARDUINO_TRAN_DATA_SIZE];
bool sentRPM = false;

void connect(ENetPeer* peer, ENetHost* host, ENetEvent event) {
	std::cout << "connect: " << peer->address.host << std::endl;
	server::send("", peer);
}
void recieve(ENetPeer* peer, ENetHost* host, ENetEvent event) {
	for (unsigned int i = 0; i < sizeof(long double); i++)
		distance.byte[i] = event.packet->data[i];
	SmartDashboard::PutNumber("distance", distance.distance);
	//std::cout << distance.distance << std::endl;
	server::send("", peer);
}
void disconnect(ENetPeer* peer, ENetHost* host, ENetEvent event) {
	std::cout << "disconnect" << std::endl;
}

void SendShootRPM() {
	//Shooter_RPM = distance.distance;
	std::cout << "Sending rpms" << std::endl;
	Shooter_RPM = SmartDashboard::GetNumber("rpms", 0);
	transmit_data[0] = 'S';
	transmit_data[1] = 'H';
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

float ReadI2C(bool m
#ifdef TWOVAR
		, bool k = false) {
#else
	) {
#endif
	uint8_t data[ARDUINO_REC_DATA_SIZE];
	for(unsigned i = 0; i < ARDUINO_REC_DATA_SIZE; i++)
		data[i] = 0;
	wire->ReadOnly(ARDUINO_REC_DATA_SIZE, data);
	std::cout << "recieved:" << data << std::endl;
#ifdef TWOVAR
	if (k) {
		code2[0] = data[0];
		code2[1] = data[1];
		;
	} else {
#endif
		code[0] = data[0];
		code[1] = data[1];
#ifdef TWOVAR
	}
#endif
#ifdef TWOVAR
	if (k)
		for (int i = 0; i < 4; i++)
			Power.Byte[i] = data[i + 2];
	else {
#endif
		for (int i = 0; i < 4; i++)
			Recieve_DataR.Byte[i] = data[i + 2];
		for (int i = 0; i < 4; i++)
			Recieve_DataL.Byte[i] = data[i + 6];
#ifdef TWOVAR
	}
#endif
	//std::cout << std::endl;
	if (m && Recieve_DataR.Double > MAX_POWER) {
		Recieve_DataR.Double = MAX_POWER;
	}
	if (m && Recieve_DataL.Double > MAX_POWER) {
		Recieve_DataL.Double = MAX_POWER;
	}
#ifdef TWOVAR
	if (m && Power.Double > MAX_POWER) {
		Power.Double = MAX_POWER;
	}
#endif
#ifdef TWOVAR
	if (k)
		SmartDashboard::PutString("Code", code2);
	else
#endif
		SmartDashboard::PutString("Code", code);
	SmartDashboard::PutNumber("Time", Timer::GetMatchTime());
#ifdef TWOVAR
	return Power.Double;
#else
	return Recieve_DataR.Double;
#endif
}

void shooter::init() {
	m_launcher = SHOOTER_MOTOR;
	m_primer = SHOOTER_PRIME_MOTOR;
	m_agitator = AGITATOR_MOTOR;
	wire = new I2C(I2C::kMXP, ARDUINO_ID);
	server::connect(connect);
	server::disconnect(disconnect);
	server::recieve(recieve);
	server::init(25572);
	atexit(server::quit);
	Recieve_DataR.Double = 0;
	distance.distance = 114.5;
	SmartDashboard::PutNumber("rpms", 20000.0);
	SmartDashboard::PutNumber("distance", 0.0);
}

void shooter::shoot() {
	if (!sentRPM) {
		delete wire;
		wire = new I2C(I2C::kMXP, ARDUINO_ID);
		SendShootRPM();
		sentRPM = true;
		code[0] = 'R';
	}
	m_launcher->Set(-ReadI2C(true
#ifdef TWOVAR
			, true
#endif
			));
#ifdef TWOVAR
	SmartDashboard::PutNumber("power", Power.Double);
#else
	SmartDashboard::PutNumber("power", Recieve_DataR.Double);
#endif
#ifdef TWOVAR
	if (code2[0] == 'I' && code2[1] == 'N') {
#else
		if (code[0] == 'I' && code[1] == 'N') {
#endif
		m_agitator->Set(frc::Relay::kOn);
		m_primer->Set(0.4);
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

void autonomous::auto1(RobotBase *rb) {
	gear::close();
	transmit_data[0] = 'A';
	transmit_data[1] = '1';
	transmit_data[2]=0;
	transmit_data[3]=0;
	wire->WriteBulk(transmit_data, ARDUINO_TRAN_DATA_SIZE);
	while (rb->IsEnabled() && rb->IsAutonomous()) {
		ReadI2C(false);
		SmartDashboard::PutNumber("LeftPower", Recieve_DataL.Double);
		SmartDashboard::PutNumber("RightPower", Recieve_DataR.Double);
		SmartDashboard::PutString("Code", code);
		if (code[0] == 'T' && code[1] == 'R')
			break;
		if (code[0] == 'E' && code[1] == 'N') {
			drivetrain::drive_lr((double) Recieve_DataL.Double,
					(double) Recieve_DataR.Double, -1.0);
		} else {
			drivetrain::drive_lr(0, 0, -1);
		}
	}
	std::cout << "finished" << std::endl;
	StopShootRPMCycle();
	gear::open();
	Wait(0.5);
	drivetrain::drive_lr(0.4, 0.4, -1);
	Wait(0.5);
	drivetrain::drive_lr(0, 0, -1);
	drivetrain::drive_lr((double) 0, (double) 0, -1.0);
}

void autonomous::auto2(RobotBase *rb) {
	transmit_data[0] = 'A';
	transmit_data[1] = '2';
	wire->WriteBulk(transmit_data, ARDUINO_TRAN_DATA_SIZE);
	while (rb->IsEnabled() && rb->IsAutonomous()) {

	}
	transmit_data[0] = 'S';
	transmit_data[1] = 'T';
	wire->WriteBulk(transmit_data, ARDUINO_TRAN_DATA_SIZE);
}
