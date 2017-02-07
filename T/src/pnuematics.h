#ifndef SRC_PNUEMATICS_H_
#define SRC_PNUEMATICS_H_
#include <DoubleSolenoid.h>
#include "WPILib.h"
namespace p {
void init();
void openGear();
void stopGear();
void closeGear();
void liftUp();
void stopLift();
void liftDown();
void prime();
void latch();
frc::Compressor* getCompressor();
frc::DoubleSolenoid* getVersa();
frc::DoubleSolenoid* getPrimer();
frc::DoubleSolenoid* getGear();
}

#endif /* SRC_PNUEMATICS_H_ */
