#ifndef SRC_FRC5572Controller_H_
#define SRC_FRC5572Controller_H_

#include <utility>
#include <GenericHID.h>

class FRC5572Controller {
public:
	FRC5572Controller(int I);
	~FRC5572Controller();
	double LT();
	bool LB();
	double RT();
	bool RB();
	bool X();
	bool Y();
	bool A();
	bool B();
	std::pair<double, double> L();
	std::pair<double, double> R();
	unsigned POV();
	bool start();
	bool back();
	bool Lbutton();
	bool Rbutton();
	void rumble(double, double);
private:
	frc::GenericHID *pad;
};

#endif
