#include "WPILib.h"
#include "drivetrain.h"
#include <I2C.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "enet.h"
#include "controller.h"
#include <iostream>
//I2C Debug Constants
#define TRAN_DATA_SIZE 4
#define REC_DATA_SIZE 4
#define DEBUG 0

union S {
	enet_uint8 byte[sizeof(long double)];
	long double distance;
};

volatile S distance;

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

class Robot: public SampleRobot {
	VictorSP *m_launcher;
	FRC5572Controller driver;
	FRC5572Controller operat;
	const int ShootVictor = 2;
	//I2C Constants/Variables
	I2C* wire;
	int arduinoID = 8;
	long Shooter_RPM = 0; //Speed i want
	unsigned char transmit_data[TRAN_DATA_SIZE];
	union {
		float Shooter_Power;
		uint8_t Shooter_Power_Byte[REC_DATA_SIZE];
	} Recieve_Data;
	float maxShooterPower = 0.75f;bool sentRPM = false;
public:
	Robot() :
			driver(0), operat(1) {
		drivetrain::setMotors<VictorSP>( { 0 }, { 3 });
		m_launcher = new VictorSP(ShootVictor);
		SmartDashboard::PutNumber("power", 0);
		SmartDashboard::PutNumber("RPM", 0);
		SmartDashboard::PutNumber("Distance", 0);
		wire = new I2C(I2C::kMXP, arduinoID);
		server::connect(connect);
		server::disconnect(disconnect);
		server::recieve(recieve);
		server::init(25572);
		atexit(server::quit);
	}
	void RobotInit() override {
	}
	void pnuematics(){

	}
	void launcherShoot() {
		if (operat.A()) {
			if (!sentRPM
					|| Shooter_RPM != SmartDashboard::GetNumber("RPM", 0)) {
				SendShootRPM();
				sentRPM = true;
			}
			m_launcher->Set(ReadShooterRPM());
		} else {
			if (sentRPM) {
				StopShootRPMCycle();
				sentRPM = false;
			}
			m_launcher->Set(0);
		}
	}
	void OperatorControl() override {
		while (IsOperatorControl() && IsEnabled()) {
			drivetrain::drive_tank(-driver.L().first, -driver.L().second, 0.5);
			launcherShoot();
			pneumatics();
			Wait(0.005); // Wait 5ms for the next update.
		}

	}
	void Autonomous() {
	}
	void SendShootRPM() {
		if (Shooter_RPM != SmartDashboard::GetNumber("RPM", 0))
			Shooter_RPM = SmartDashboard::GetNumber("RPM", 0);
		transmit_data[0] = 'S';
		transmit_data[1] = 'T';
		transmit_data[2] = Shooter_RPM >> 8;
		transmit_data[3] = Shooter_RPM;
		wire->WriteBulk(transmit_data, TRAN_DATA_SIZE);
	}
	void StopShootRPMCycle() {
		transmit_data[0] = 'S';
		transmit_data[1] = 'P';
		transmit_data[2] = 0;
		transmit_data[3] = 0;
		wire->WriteBulk(transmit_data, TRAN_DATA_SIZE);
	}
	float ReadShooterRPM() {
		wire->ReadOnly(REC_DATA_SIZE, Recieve_Data.Shooter_Power_Byte);
		if (Recieve_Data.Shooter_Power > maxShooterPower) {
			Recieve_Data.Shooter_Power = maxShooterPower;
		}
		//Recieve_Data.Shooter_Power = Recieve_Data.Shooter_Power  > maxShooterPower ? maxShooterPower : Recieve_Data.Shooter_Power;
		if (DEBUG) {
			std::cout << "Power Recieved: ";
			std::printf("%f", Recieve_Data.Shooter_Power);
			std::cout << std::endl;
		}
		SmartDashboard::PutNumber("power", Recieve_Data.Shooter_Power);
		return Recieve_Data.Shooter_Power;
	}
	void Test() override {
		LiveWindow::GetInstance()->SetEnabled(false);
		while (IsEnabled()) {
			SmartDashboard::PutNumber("Distance", distance.distance);
		}
	}
	void Disabled() override {
	}
};

START_ROBOT_CLASS(Robot)
