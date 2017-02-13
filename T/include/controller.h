#ifndef SRC_FRC5572Controller_H_
#define SRC_FRC5572Controller_H_

#include <utility>
#include <thread>
#include <XboxController.h>

inline void nothingb() {}
inline void nothinga(double) {}
inline void nothingj(double, double) {}

class FRC5572Controller {
public:
	typedef void(*ButtonEvent)();
	typedef void(*AxisEvent)(double);
	typedef void(*JoystickEvent)(double, double);
	FRC5572Controller(int I);
	~FRC5572Controller();
	double LT();
	inline void LT(AxisEvent ae){
		lt = ae;
	}
	bool LB();
	inline void LB(ButtonEvent ae){
		lb = ae;
	}
	double RT();
	inline void RT(AxisEvent ae){
		rt = ae;
	}
	bool RB();
	inline void RB(ButtonEvent ae){
		rb = ae;
	}
	bool X();
	inline void X(ButtonEvent ae){
		x = ae;
	}
	bool Y();
	inline void Y(ButtonEvent ae){
		y = ae;
	}
	bool A();
	inline void A(ButtonEvent ae){
		a = ae;
	}
	bool B();
	inline void B(ButtonEvent ae){
		b = ae;
	}
	std::pair<double, double> L();
	inline void L(JoystickEvent ae){
		l = ae;
	}
	std::pair<double, double> R();
	inline void R(JoystickEvent ae){
		r = ae;
	}
	unsigned POV();
	bool start();
	bool back();
	bool Lbutton();
	bool Rbutton();
	void rumble(double, double);
	void runEvents();
private:
	frc::XboxController *pad;
	AxisEvent lt = nothinga;
	ButtonEvent lb = nothingb;
	AxisEvent rt = nothinga;
	ButtonEvent rb = nothingb;
	ButtonEvent x = nothingb;
	ButtonEvent y = nothingb;
	ButtonEvent a = nothingb;
	ButtonEvent b = nothingb;
	JoystickEvent l = nothingj;
	JoystickEvent r = nothingj;
	std::thread s;
	bool k = true;
};

#endif
