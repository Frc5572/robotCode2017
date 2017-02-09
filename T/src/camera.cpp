#include <CameraServer.h>
#include "camera.h"

cs::UsbCamera uc;

void camera::init() {
	 uc = frc::CameraServer::GetInstance()->StartAutomaticCapture();
}

void camera::brightness(int int1) {
	uc.SetBrightness(int1);
}

void camera::exposure(int int1) {
	uc.SetExposureManual(int1);
}

void camera::resolution(int int1, int int2) {
	uc.SetResolution(int1, int2);
}

void camera::exposure_auto() {
	uc.SetExposureAuto();
}

void camera::white_balance_auto() {
	uc.SetWhiteBalanceAuto();
}

void camera::white_balance(int int1) {
	uc.SetWhiteBalanceManual(int1);
}

void camera::fps(int int1) {
	uc.SetFPS(int1);
}
