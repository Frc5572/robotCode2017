/*
 * ArduinoI2C.h
 *
 *  Created on: Jan 26, 2017
 *      Author: stem
 */

#ifndef ARDUINOI2C_H_
#define ARDUINOI2C_H_
#include <string>
namespace Arduino {
static int arduinoI2Creg = 84; //Arduino address
static int rioI2Creg = 0x01; //RoboRio address
void initialize();
void Write(std::string);
void Write(float);
void Write(double);
void Write(int);
float ReadFloat();
int ReadInt();
std::string ReadData();
}

#endif /* SRC_ARDUINOI2C_H_ */
