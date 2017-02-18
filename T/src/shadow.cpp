#include "shadow.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <CANTalon.h>
#include <stdint.h>
#include <drivetrain.h>
#include <gear.h>
#include <shooter.h>
#include <Timer.h>
#include <iostream>
#include <stdlib.h>
#define DT_ID	0b001
#define S_ID	0b010
#define G_ID	0b100
//Id->Timestamp->Value
int fd;
bool writing;
frc::Timer clock_;

void* dtob(double a) {
	static union {
		double d;
		void* v;
	} b;
	b.d = a;
	return b.v;
}

void quit() {
	close(fd);
}

void shadow::init() {
	fd = open("shadow.dat", O_RDWR);
	atexit(quit);
}

void shadow::dtwrite(double a, double b, double c) {
	drivetrain::drive_tank(a, b, c);
	if (writing) {
		write(fd, (const void*) DT_ID, 1);
		write(fd, dtob(clock_.Get()), sizeof(clock_.Get()));
		write(fd, dtob(a), sizeof(double));
		write(fd, dtob(b), sizeof(double));
		write(fd, dtob(c), sizeof(double));
	}
}

void shadow::swrite(bool b) {
	if (b) {
		shooter::shoot();
	} else {
		shooter::stop();
	}
	if (writing) {
		write(fd, (const void*) S_ID, 1);
		write(fd, dtob(clock_.Get()), sizeof(clock_.Get()));
		write(fd, (const void*) (b), sizeof(bool));
	}
}

void shadow::gwrite(bool b) {
	if (b)
		gear::open();
	else
		gear::close();
	if (writing) {
		write(fd, (const void*) G_ID, 1);
		write(fd, dtob(clock_.Get()), sizeof(clock_.Get()));
		write(fd, (const void*) (b), sizeof(bool));
	}
}

void shadow::start() {
	clock_.Start();
	writing = true;
}

void shadow::stop() {
	clock_.Stop();
	writing = false;
}
void shadow::run(RobotBase *rb) {
	Timer v;
	v.Start();
	while (rb->IsAutonomous() && rb->IsEnabled()) {
		uint8_t type;
		read(fd, &type, 1);
		double timestamp;
		read(fd, &timestamp, sizeof(double));
		while (v.Get() < timestamp)
			;
		if (type == DT_ID) {
			double a, b, c;
			read(fd, &a, sizeof(double));
			read(fd, &b, sizeof(double));
			read(fd, &c, sizeof(double));
			drivetrain::drive_tank(a, b, c);
		} else if (type == S_ID) {
			bool b;
			read(fd, &b, sizeof(bool));
			if (b) {
				shooter::shoot();
			} else {
				shooter::stop();
			}
		} else if (type == G_ID) {
			bool b;
			read(fd, &b, sizeof(bool));
			if (b)
				gear::open();
			else
				gear::close();
		} else {
			std::cerr << "Wrong offset!!!" << std::endl;
		}
	}
}
