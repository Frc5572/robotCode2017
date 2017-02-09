#ifndef SRC_PORTS_H_
#define SRC_PORTS_H_

#include <CANTalon.h>
#include <VictorSP.h>
#include <Talon.h>
#include <TalonSRX.h>
#include <DoubleSolenoid.h>


#define GEAR_DOUBLE_SOLENOID  2,5
#define VERSA_DOUBLE_SOLENOID 0,7
#define CLIMBER_DOUBLE_SOLENOID 1,6
#define CLIMBER_MOTOR_TYPE frc::VictorSP
#define CLIMBER_MOTOR new CLIMBER_MOTOR_TYPE(3)
#define SHOOTER_MOTOR_TYPE frc::VictorSP
#define SHOOTER_MOTOR new SHOOTER_MOTOR_TYPE(3);
#define ARDUINO_ID 8
#define MAX_SHOOTER_POWER 0.75
#define ARDUINO_TRAN_DATA_SIZE 4
#define ARDUINO_REC_DATA_SIZE 4
#define ARDUINO_DEBUG 0

#endif
