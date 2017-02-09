/*
 * ArduinoI2C.cpp
 *
 *  Created on: Jan 26, 2017
 *      Author: stem
 */
#include "ArduinoI2C.h"
#include "I2C.h"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
I2C *swire;
int packetSize = 13;
uint8_t *i2c_buffer = new uint8_t[packetSize];
void write_string(std::string);

void Arduino::initialize() {
	swire = new I2C(I2C::Port::kMXP, Arduino::arduinoI2Creg);
	write_string("");
}
void Arduino::Write(std::string d) {
	std::string data = std::string("S") + d;
}
void Arduino::Write(float d) {
	std::stringstream ss;
	ss << d;
	std::string m;
	ss >> m;
	std::string data = std::string("A") + m;
	write_string(data);
}
void Arduino::Write(double d) {
	std::stringstream ss;
	ss << d;
	std::string m;
	ss >> m;
	std::string data = std::string("A") + m;
	write_string(data);
}
void Arduino::Write(int d) {
	std::stringstream ss;
	ss << d;
	std::string m;
	ss >> m;
	std::string data = std::string("B") + m;
	write_string(data);
}
std::string Arduino::ReadData() {
	std::stringstream ss;
	swire->Read(Arduino::rioI2Creg, packetSize, i2c_buffer);
	for (int i = 0; i < packetSize; i++)
		ss << i2c_buffer[i];
	std::string s;
	ss >> s;
	if (s.c_str()[0] == 'S') {
		std::string m = s.substr(1);
		return m;
	} else {
		return "\0";
	}
}
float Arduino::ReadFloat() {
	std::stringstream ss;
	swire->Read(Arduino::rioI2Creg, packetSize, i2c_buffer);
	for (int i = 0; i < packetSize; i++)
		ss << i2c_buffer[i];
	std::string s;
	ss >> s;
	//std::cout << "size:" << s.size() << ";\n val:" << s << std::endl;
	if (s.c_str()[0] == 'A') {
		//std::cout << "Read 'A'" << std::endl;
		float m = std::stof(s.substr(1));
		return m;
		//std::cout << s << std::endl;
		//return -1;
	} else {
		return -1;
	}
}
int Arduino::ReadInt() {
	std::stringstream ss;
	swire->Read(Arduino::rioI2Creg, packetSize, i2c_buffer);
	for (int i = 0; i < packetSize; i++)
		ss << i2c_buffer[i];
	std::string s;
	ss >> s;
	if (s.c_str()[0] == 'B') {
		int m = std::stoi(s.substr(1));
		return m;
	} else {
		return -1;
	}

}
void write_string(std::string m) {
	const char *val = m.c_str();
	unsigned length = m.length();
	uint8_t *sn = new uint8_t[length + 1];
	sn[0] = Arduino::rioI2Creg;
	for (unsigned i = 0; i < length; i++)
		sn[i + 1] = val[i];
	swire->WriteBulk(sn, length + 1);
}
