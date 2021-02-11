#include "SYSTEM.hpp"
#include "matplotlibcpp.h"
using namespace std;

int main()
{

// Initialize SYSTEM object
	SYSTEM system;

// Initialize a bag to calibrate the sensors containing 3 markers
	BAG calibrator(3);
	calibrator.markers[0].position = {0, 0.1, 0.1};
	calibrator.markers[1].position = {0.05, -0.05, 0.1};
	calibrator.markers[2].position = {0.1, -0.05, 0};

// Calibrate the Sensor System
	system.calibrate(calibrator);

}


