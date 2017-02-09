/*
 * camera.h
 *
 *  Created on: Feb 8, 2017
 *      Author: rosbots
 */

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_

namespace camera {
void init();
void brightness(int);
void exposure(int);
void resolution(int, int);
void exposure_auto();
void white_balance_auto();
void white_balance(int);
void fps(int);
}

#endif /* SRC_CAMERA_H_ */
