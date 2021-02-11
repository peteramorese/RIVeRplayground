#include "CONSTANTS.hpp"
#include "CORE.hpp"
#include "DATA.hpp"
#include "MARKER.hpp"
#include "BAG.hpp"
#include "SENSOR.hpp"
#include "EKF.hpp"
// #include "addtl_func.hpp"

#include "matplotlibcpp.h"

#include <iostream>
#include <string>
#include <vector>
#include <armadillo>

using namespace std;
using namespace arma;
namespace plt = matplotlibcpp;


class SYSTEM
{
public:
	int sim = 1; // Flag defining simulation run mode
	
	SYSTEM();
	~SYSTEM();
	void calibrate(BAG);


private:
	CORE core; // Object defining the core frame
	CONSTANTS cnst; // Object defining system-wide constants
	vector<SENSOR> sensors; // Vector defining the network of sensors
	BAG bag; // Object defining the cargo bag being estimated

	void init_default_sensors();
	vector<vector<DATA>> read_data(string);


};