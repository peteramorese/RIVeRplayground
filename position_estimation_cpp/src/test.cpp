#include "CONSTANTS.hpp"
#include "CORE.hpp"
#include "DATA.hpp"
#include "MARKER.hpp"
#include "BAG.hpp"
#include "SENSOR.hpp"
#include "EKF.hpp"
#include "addtl_func.hpp"

#include "matplotlibcpp.h"

#include <iostream>
#include <string>
#include <armadillo>

using namespace std;
using namespace arma;
namespace plt = matplotlibcpp;


int main()
{

// Initialize CONSTANTS object
	CONSTANTS cnst;
	
// Initialize CORE object
	CORE core;

// Initialize a bag to calibrate the sensors containing 3 markers
	BAG calibrator(3);
	calibrator.markers[0].position = {0, 0.1, 0.1};
	calibrator.markers[1].position = {0.05, -0.05, 0.1};
	calibrator.markers[2].position = {0.1, -0.05, 0};

// Initialize the sensors
	std::vector<SENSOR> SENSORS;
	SENSOR S;

	for(int i = 0; i < cnst.m; i++)
	{
		SENSORS.push_back(S);

		SENSORS[i].init_default_sensors(i, core);
	}

// Initialize the time vector
	double t_start = 0.0;
	double t_stop = 120.0;

	vector<double> t = get_time_vector(t_start, t_stop, SENSORS[0].dT);

// Initialize the simulated data vector
	std::vector<std::vector<std::vector<DATA>>> Y;
	// Y[sid][time][mid]
	std::vector<std::vector<DATA>> Y_sid;

// Read simulated pixy data

	string filename;

	for(int i = 0; i < cnst.m; i++)
	{
		filename = "../data/Y_sim_.txt";
		filename[13] = to_string(i+1)[0];

		Y_sid = read_data(filename);
		Y.push_back(Y_sid);
	}

// Calibrate sensors
	for(int i = 0; i < cnst.m; i++)
	{
		cout << "Calibrating Sensor " << i << endl;
		SENSORS[i].calibrate_sensor(t, Y[i], calibrator, core, cnst);
		// SENSORS[i].plot_ekf(t);
	}

}


