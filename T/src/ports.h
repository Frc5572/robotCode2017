#ifndef SRC_PORTS_H_
#define SRC_PORTS_H_

#include <CANTalon.h>
#include <CanTalonSRX.h>
#include <VictorSP.h>
#include <Talon.h>
#include <TalonSRX.h>
#include <DoubleSolenoid.h>
#include <Relay.h>

#define CLIMBER_MOTOR_TYPE CanTalonSRX
#define SHOOTER_MOTOR_TYPE CanTalonSRX
#define SHOOTER_PRIME_MOTOR_TYPE CanTalonSRX
#define INTAKE_MOTOR_TYPE CanTalonSRX
#define AGITATOR_MOTOR_TYPE frc::Relay
#define DRIVETRAIN_MOTOR_TYPE frc::VictorSP

#define GEAR_DOUBLE_SOLENOID_ID  1,6
#define VERSA_DOUBLE_SOLENOID_ID 0,7
#define CLIMBER_DOUBLE_SOLENOID_ID 2,5
#define CLIMBER_MOTOR_ID 2

// Shooter definitions
#define SHOOTER_MOTOR_ID 7			//
#define SHOOTER_PRIME_MOTOR_ID 4	//
#define MAX_POWER_SHOOTER 1			// Max value for shooter motor 1 = 100%


#define AGITATOR_MOTOR_ID 0, frc::Relay::kForwardOnly
#define INTAKE_MOTOR_ID 8
#define DRIVETRAIN_MOTOR_LEFT_ID 0,1,2
#define DRIVETRAIN_MOTOR_RIGHT_ID 3,4,5


#define CLIMBER_MOTOR new CLIMBER_MOTOR_TYPE(CLIMBER_MOTOR_ID)
#define SHOOTER_MOTOR new SHOOTER_MOTOR_TYPE(SHOOTER_MOTOR_ID)
#define SHOOTER_PRIME_MOTOR new SHOOTER_PRIME_MOTOR_TYPE(SHOOTER_PRIME_MOTOR_ID)
#define AGITATOR_MOTOR new AGITATOR_MOTOR_TYPE(AGITATOR_MOTOR_ID)
#define INTAKE_MOTOR new INTAKE_MOTOR_TYPE(INTAKE_MOTOR_ID)



// I2C Communication definitions
#define ARDUINO_ID 8
#define ARDUINO_TRAN_DATA_SIZE 4
#define ARDUINO_REC_DATA_SIZE 14

#define ARDUINO_DEBUG 0
#define DEBUG 1

#endif
