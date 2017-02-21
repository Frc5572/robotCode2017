/*
 * shooter.h
 *
 *  Created on: Feb 8, 2017
 *      Author: rosbots
 */

#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_

#include <RobotBase.h>

namespace shooter {
void init();
void shoot();
void stop();
}

namespace autonomous {
void auto1(RobotBase *rb); // Move forward
void auto2(RobotBase *rb); //
}

#endif /* SRC_SHOOTER_H_ */
