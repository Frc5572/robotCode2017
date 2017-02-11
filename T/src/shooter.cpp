#include "shooter.h"
#include <I2C.h>
#include <iostream>
#include <enet.h>
#include "ports.h"
#include <DynamicLookupTable.h>

union S {
	enet_uint8 byte[sizeof(long double)];
	long double distance;
};
volatile S distance;
union {
	float Shooter_Power;
	uint8_t Shooter_Power_Byte[ARDUINO_REC_DATA_SIZE];
} Recieve_Data;

SHOOTER_MOTOR_TYPE *m_launcher;
I2C* wire;

long Shooter_RPM = 0; //Speed i want
unsigned char transmit_data[ARDUINO_TRAN_DATA_SIZE];
bool sentRPM = false;

void connect(ENetPeer* peer, ENetHost* host, ENetEvent event) {
	std::cout << "connect: " << peer->address.host << std::endl;
	server::send("", peer);
}
void recieve(ENetPeer* peer, ENetHost* host, ENetEvent event) {
	for (unsigned int i = 0; i < sizeof(long double); i++)
		distance.byte[i] = event.packet->data[i];
	std::cout << distance.distance << std::endl;
	server::send("", peer);
}
void disconnect(ENetPeer* peer, ENetHost* host, ENetEvent event) {
	std::cout << "disconnect" << std::endl;
}

void SendShootRPM() {
	Shooter_RPM = distance.distance;
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
	wire->ReadOnly(ARDUINO_REC_DATA_SIZE, Recieve_Data.Shooter_Power_Byte);
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
	wire = new I2C(I2C::kMXP, ARDUINO_ID);
	server::connect(connect);
	server::disconnect(disconnect);
	server::recieve(recieve);
	server::init(25572);
	atexit(server::quit);
}

void shooter::shoot() {
	if (!sentRPM) {
		SendShootRPM();
		sentRPM = true;
	}
	m_launcher->Set(ReadShooterRPM());
}

void shooter::stop() {
	if (sentRPM) {
		StopShootRPMCycle();
		sentRPM = false;
	}
	m_launcher->Set(0);
}
