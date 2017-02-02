#include "WPILib.h"
#include "drivetrain.h"
#include <I2C.h>
#include <stdlib.h>
#include <sstream>
#include <string>
//I2C Debug Constants
#define TRAN_DATA_SIZE 4
#define REC_DATA_SIZE 4
#define DEBUG 0
class Robot: public SampleRobot {
	Joystick m_stick1;
	Joystick m_stick2;
	VictorSP *m_launcher;
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
	float maxShooterPower = 0.75f;
	bool sentRPM = false;
public:
	Robot() :
			m_stick1(0), m_stick2(1) {
		drivetrain::setMotors<VictorSP>( { 0 }, { 3 });
		m_launcher = new VictorSP(ShootVictor);
		SmartDashboard::PutNumber("power", 0);
		SmartDashboard::PutNumber("RPM", 0);
		wire = new I2C(I2C::kMXP, arduinoID);
	}
	void RobotInit() override {
	}
	void OperatorControl() override {
		while (IsOperatorControl() && IsEnabled()) {
			drivetrain::drive_tank(-m_stick1.GetX(), -m_stick1.GetY(),
					0.5 + (m_stick1.GetThrottle() / 2));
			if (m_stick2.GetTrigger()) {
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
		while (IsEnabled()) {
			Wait(5);
			SendShootRPM();
			Wait(5);
			ReadShooterRPM();
		}
		StopShootRPMCycle();
	}
	void Disabled() override {
	}
};

/*
 //uint8_t b[12];
 //int sie=wire->Read(rioI2Creg, 1, s.a);
 std::stringstream ss;
 wire->Read(rioI2Creg, packetSize, i2c_buffer);
 for(int i=0;i<packetSize;i++)
 ss << i2c_buffer[i];
 std::string s;
 ss >> s;
 std::cout << "size:" << s.size() << ";\n val:" << s << std::endl;
 if(s.c_str()[0]=='A'){
 std::cout<<"Read 'A'" << std::endl;
 float m = std::stof(s.substr(1));
 std::cout << m;
 std::printf("\n");
 }else{
 std::cout<<"Value is not a float" << std::endl;
 std::printf("\n");
 }

 Write("Hello");
 */

START_ROBOT_CLASS(Robot)
