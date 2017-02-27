/*
 * shooter.h
 *
 *  Created on: Feb 8, 2017
 *      Author: rosbots
 */

#ifndef SRC_SHOOTER_H_
#define SRC_SHOOTER_H_
#include <I2C.h>
#include <RobotBase.h>

namespace shooter{
void init();
void shoot();
void stop();
}

static I2C *wire;

namespace autonomous {
	void auto1(RobotBase *rb); // Move forward
	void auto2(RobotBase *rb); // Turn
	void auto3(RobotBase *rb); // Move forward
	void auto4(RobotBase *rb); // Turn
	void auto5(RobotBase *rb); // Turn
	void DriveStraight(RobotBase *rb, char mode); //Move on a straight line
	void Rotate(RobotBase *rb, char mode); //Rotate the robot
	void DriveBack(); // Move robot back after releasing the gear
}

#define INDEXER_POWER 	0.4 //Indexer power set to 40%

#endif /* SRC_SHOOTER_H_ */
