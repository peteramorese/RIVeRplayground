#include "CONSTANTS.hpp"
#include "CORE.hpp"
#include "DATA.hpp"
#include "MARKER.hpp"
#include "BAG.hpp"
#include "SENSOR.hpp"
#include "SENSOR_MIN.hpp"
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
	bool sim; // Flag defining simulation run mode

	SYSTEM(bool);
	~SYSTEM();
	void calibrate(BAG);
	void run_estimator();

	void get_data();

private:
	CORE core; // Object defining the core frame
	CONSTANTS cnst; // Object defining system-wide constants
	vector<SENSOR> sensors; // Vector defining the network of sensors
	vector<SENSOR_MIN> sensors_min; // Vector defining the network of sensors (minimal)
	BAG bag; // Object defining the cargo bag being estimated
	vector<vector<vector<DATA>>> Y; // Vector to store data in

	void init_default_sensors();
	
	vector<vector<DATA>> read_data(string);
	void set_sensors_min();
	vector<vector<vector<DATA>>> reorg_data();


};