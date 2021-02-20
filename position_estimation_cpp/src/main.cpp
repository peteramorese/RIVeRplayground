#include "SYSTEM.hpp"
// #include "matplotlibcpp.h"
using namespace std;

int main()
{

// Initialize SYSTEM object
	bool sim = true; // Set run mode to simulation
	bool plot = true; // Set the plot flag
	SYSTEM system(sim, plot);

// Initialize a bag to calibrate the sensors containing 3 markers
	BAG calibrator(6);
	calibrator.markers[0].position = {0, 0.1, 0.1};
	calibrator.markers[1].position = {0.05, -0.05, 0.1};
	calibrator.markers[2].position = {0.1, -0.05, 0};
	// calibrator.markers[3].position = {2.25, 0.85, -0.80};
	// calibrator.markers[4].position = {2.50, 0.90, -0.8};
	// calibrator.markers[5].position = {2.30, 0.75, -0.75};
	calibrator.markers[3].position = {2.25 - 2.272, 0.85 - 0.8453, -0.80 + 0.8453};
	calibrator.markers[4].position = {2.50 - 2.272, 0.90 - 0.8453, -0.8 + 0.8453};
	calibrator.markers[5].position = {2.30 - 2.272, 0.75 - 0.8453, -0.75 + 0.8453};

	system.assign_bag(calibrator);

// Calibrate the Sensor System
	system.calibrate(calibrator);

// Run the bag estimator
	// system.run_estimator();

}


