#include "shooter.h"
#include <I2C.h>
#include <iostream>
#include <enet.h>
#include "ports.h"
#include <DynamicLookupTable.h>
#include <cmath>
#include <Timer.h>

union S {
	enet_uint8 byte[sizeof(long double)];
	long double distance;
};
volatile S distance;
AGITATOR_MOTOR_TYPE *m_agitator;

union {
	float Shooter_Power;
	uint8_t Shooter_Power_Byte[ARDUINO_REC_DATA_SIZE];
} Recieve_Data;

SHOOTER_MOTOR_TYPE *m_launcher;
SHOOTER_PRIME_MOTOR_TYPE *m_primer;
I2C* wire;
char code[2];

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
	Shooter_RPM = SmartDashboard::GetNumber("rpms", 0);
	transmit_data[0] = 'S';
	transmit_data[1] = 'T';
	transmit_data[2] = Shooter_RPM >> 8;
	transmit_data[3] = Shooter_RPM;
	wire->WriteBulk(transmit_data, ARDUINO_TRAN_DATA_SIZE);
}
void StopShootRPMCycle() {
	transmit_data[0] = 'S';
	transmit_data[1] = 'P';
	transmit_data[2] = 0;
	transmit_data[3] = 0;
	wire->WriteBulk(transmit_data, ARDUINO_TRAN_DATA_SIZE);
}
float ReadShooterRPM() {
	uint8_t data[ARDUINO_REC_DATA_SIZE];
	wire->ReadOnly(ARDUINO_REC_DATA_SIZE, data);
	code[0] = data[0];
	code[1] = data[1];
	for (int i = 0; i < 4; i++)
		Recieve_Data.Shooter_Power_Byte[i] = data[i + 2];
	//std::cout << std::endl;
	if (Recieve_Data.Shooter_Power > MAX_SHOOTER_POWER) {
		Recieve_Data.Shooter_Power = MAX_SHOOTER_POWER;
	}
	//Recieve_Data.Shooter_Power = Recieve_Data.Shooter_Power  > maxShooterPower ? maxShooterPower : Recieve_Data.Shooter_Power;
	if (ARDUINO_DEBUG) {
		std::cout << "Power Recieved: ";
		std::printf("%f", Recieve_Data.Shooter_Power);
		std::cout << std::endl;
	}
	SmartDashboard::PutNumber("power", Recieve_Data.Shooter_Power);
	return Recieve_Data.Shooter_Power;
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
	Recieve_Data.Shooter_Power = 0;
	distance.distance = 114.5;
	SmartDashboard::PutNumber("rpms", 20000.0);
}

Timer timer;
bool started = false;
bool agitate_flag = false;

void shooter::shoot() {
	if (!sentRPM) {
		SendShootRPM();
		sentRPM = true;
	}
	m_launcher->Set(-ReadShooterRPM());
	if (code[0] == 'I' && code[1] == 'N') {
		if (!started) {
			timer.Start();
			started = true;
		}
		if (timer.Get() * 2 - floor(timer.Get() * 2) > 0.5){
			m_primer->Set(0.4);
			if(agitate_flag)
				m_agitator->Set(frc::Relay::kOn);
			else
				m_agitator->Set(frc::Relay::kOff);
			agitate_flag = !agitate_flag;
		}
		else{
			m_agitator->Set(frc::Relay::kOff);
			m_primer->Set(0.0);
		}
	} else {
		m_primer->Set(0.0);
		m_agitator->Set(frc::Relay::kOff);
		if (started) {
			timer.Stop();
			timer.Reset();
		}
		started = false;
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
