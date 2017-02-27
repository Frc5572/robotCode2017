/*
 * gear.h
 *
 *  Created on: Feb 8, 2017
 *      Author: rosbots
 */

#ifndef SRC_GEAR_H_
#define SRC_GEAR_H_

#define WAIT_DROPOFF_GEAR 0.4 //Seconds to wait before moving away from peg

namespace gear {
void open();
void stop();
void close();
void init();
}

#endif /* SRC_GEAR_H_ */
