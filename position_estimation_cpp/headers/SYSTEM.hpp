#include "CONSTANTS.hpp"
#include "CORE.hpp"
#include "DATA.hpp"
#include "MARKER.hpp"
#include "BAG.hpp"
#include "SENSOR.hpp"
#include "SENSOR_MIN.hpp"
#include "EKF.hpp"

#include "oriDetermine.h"
#include "matrix.h"

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
	bool plot = false; // Flag to plot results

	SYSTEM(bool s = true, bool p = false);
	~SYSTEM();
	void set_sim_flag(bool);
	void assign_bag(BAG);
	void calibrate_pickup(BAG);
	void calibrate_dropoff(BAG);
	void run_estimator(std::vector<int>);
	vector<vector<vector<DATA>>> get_data(std::vector<int>, string, int);
	// void clear_data();


private:
	CORE core; // Object defining the core frame
	CONSTANTS cnst; // Object defining system-wide constants
	vector<SENSOR> sensors; // Vector defining the network of sensors
	vector<SENSOR_MIN> sensors_min; // Vector defining the network of sensors (minimal)
	BAG bag; // Object defining the cargo bag being estimated
	// vector<vector<vector<DATA>>> Y; // Vector to store data in

	void init_default_sensors();
	void calibrate(BAG, std::vector<int> s);
	vector<vector<DATA>> read_data(string);
	void set_sensors_min();
	vector<vector<vector<DATA>>> reorg_data(vector<vector<vector<DATA>>>);
};