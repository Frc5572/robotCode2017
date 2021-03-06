#include <CameraServer.h>
#include "camera.h"
#include <thread>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

cs::UsbCamera uc; //Front
cs::UsbCamera bc;
cs::CvSource outputStream;

volatile bool frontorback = false;

void mt() {
	//uc = frc::CameraServer::GetInstance()->StartAutomaticCapture();
	//bc = frc::CameraServer::GetInstance()->StartAutomaticCapture();

	bc = cs::UsbCamera("a", 0);
	uc = cs::UsbCamera("b", 1);
	//camera::resolution(320, 240);
	outputStream = frc::CameraServer::GetInstance()->PutVideo(
			"DriveStationVideo", 640, 480);
	cs::CvSink front = frc::CameraServer::GetInstance()->GetVideo(uc);
	cs::CvSink back = frc::CameraServer::GetInstance()->GetVideo(bc);
	cv::Mat mat;
	while (true) {
		if (!frontorback) {
			if (front.GrabFrame(mat) == 0) {
				// Send the output the error.
				outputStream.NotifyError(front.GetError());
				// skip the rest of the current iteration
				continue;
			}
		} else {
			if (back.GrabFrame(mat) == 0) {
				// Send the output the error.
				outputStream.NotifyError(back.GetError());
				// skip the rest of the current iteration
				continue;
			}
		}
		outputStream.PutFrame(mat);
	}
}

void camera::init() {
	std::thread t(mt);
	t.detach();
}

void camera::brightness(int int1) {
	uc.SetBrightness(int1);
	bc.SetBrightness(int1);
}

void camera::exposure(int int1) {
	uc.SetExposureManual(int1);
	bc.SetExposureManual(int1);
}

void camera::resolution(int int1, int int2) {
	uc.SetResolution(int1, int2);
	bc.SetResolution(int1, int2);
}

void camera::exposure_auto() {
	uc.SetExposureAuto();
	bc.SetExposureAuto();
}

void camera::white_balance_auto() {
	uc.SetWhiteBalanceAuto();
	bc.SetWhiteBalanceAuto();
}

void camera::white_balance(int int1) {
	uc.SetWhiteBalanceManual(int1);
	bc.SetWhiteBalanceManual(int1);
}

void camera::fps(int int1) {
	uc.SetFPS(int1);
	bc.SetFPS(int1);
}

void camera::switchc() {
	frontorback = !frontorback;
}

bool camera::version() {
	return frontorback;
}
