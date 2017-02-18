#ifndef SRC_SHADOW_H_
#define SRC_SHADOW_H_

#include <RobotBase.h>

namespace shadow {
void init();
void dtwrite(double, double, double);
void swrite(bool);
void gwrite(bool);
void start();
void stop();
void run(RobotBase*);
}

#endif
