#include "drivetrain.h"
#include <iostream>
#include "ports.h"
#define DEBUG 0
std::vector<SpeedController*> motors;
/*

Sets motor values for tank drive

*/
void drivetrain::setMotors(std::vector<SpeedController*> l, std::vector<SpeedController*> r){ // Set the  stored SpeedController*s to i (left) and j (right)
	motors.resize(l.size() + r.size());
	for(unsigned i = 0; i < l.size(); i++){
		motors[i] = l[i];
	}
	for(unsigned i = 0; i < r.size(); i++){
		motors[i+l.size()] = l[i];
	}
}

double signum(double x){return x > 0 ? 1 : (x < 0 ? -1 : 0);}; // Sign value. This is similar to abs(x) / x, except x=0 returns 0.

double max(double m, double n){return m > n ? m : n;} // Larger of two values

/*

Method which sets the motor outputs based on joystick orientation. 

*/
void drivetrain::drive_tank(double x, double y, double amnt){ // joystick x, joystick y, scaling modifier
	x /= amnt; // Scale
	y /= amnt; // Scale
	x = signum(x) * (x * x); // Square, retain sign
	y = signum(y) * (y * y); // Square, retain sign
	double d = signum(y) * max(y * signum(y), x * signum(x));
	if(signum(x) == signum(y)){ // If in quad I or III
		double m = y - x;
		for(unsigned i = 0; i < motors.size() / 2; i++){
			motors[i]->Set(m);
		}
		for(unsigned i = motors.size() / 2; i < motors.size(); i++){
			motors[i]->Set(d);
		}
	} else { // If in quad II or IV
		double m = y + x;
		for(unsigned i = 0; i < motors.size() / 2; i++){
			motors[i]->Set(d);
		}
		for(unsigned i = motors.size() / 2; i < motors.size(); i++){
			motors[i]->Set(m);
		}
	}
	if(DEBUG)
		std::cout << motors[0]->Get() << "," << motors[1]->Get() << std::endl;
}

frc::DoubleSolenoid *versa;

void drivetrain::drop_versa() {
	versa->Set(DoubleSolenoid::kForward);
}

void drivetrain::init() {
	versa = new DoubleSolenoid(12, VERSA_DOUBLE_SOLENOID);
}

void drivetrain::retract_versa() {
	versa->Set(DoubleSolenoid::kReverse);
}
