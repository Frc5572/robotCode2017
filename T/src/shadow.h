#include <CANTalon.h>
#include <CanTalonSRX.h>
#include <VictorSP.h>
#include <Talon.h>
#include <TalonSRX.h>
#include <DoubleSolenoid.h>
#include <Relay.h>
#include <fstream>

#ifndef SRC_SHADOW_H_
#define SRC_SHADOW_H_

template<typename T>
class shadow_writer {
public:
	shadow_writer(T t, unsigned id, std::ofstream file) :
			controller(t), id(id), out(file) {}
	~shadow_writer() {}
	void update(){
		out << id << controller.Get();
	}
private:
	T controller;
	unsigned id;
	std::ofstream out;
};


class shadow_reader {
public:

private:

};

#endif
