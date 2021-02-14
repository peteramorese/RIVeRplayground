#include "SYSTEM.hpp"

#include <armadillo>
using namespace arma;
using namespace std;


SYSTEM::SYSTEM(bool s, bool p)
{
	// Initializes the SYSTEM object

	cout << "Initializing System...\t\t";

	sim = s; // Set the software run mode
	plot = p; // Set the plotting flag

	init_default_sensors(); // Initialize the default sensor network

	set_sim_flag(s); // Propagate the software run mode setting

	cout << "DONE" << endl;
};


SYSTEM::~SYSTEM(){};


void SYSTEM::init_default_sensors()
{
	// Initializes the nominally defined sensor network

	SENSOR S;

	for(int i = 0; i < cnst.m; i++) // For each sensor
	{
		sensors.push_back(S); // Add sensor to the network
		sensors[i].sim = sim; // Set the software run mode
		sensors[i].init_default_sensor(i, core); // Set the default settings
	}
}


void SYSTEM::set_sim_flag(bool s)
{
	// Propagates the software run mode through the SYSTEM object fields
	// 
	// Inputs:
	// 		s - [bool] Flag defining the software run mode
	// 

	sim = s;
	
	for(int i = 0; i < bag.markers.size(); i++)
	{
		bag.markers[i].sim = sim;
	}

	for(int i = 0; i < sensors.size(); i++)
	{
		sensors[i].sim = sim;
	}

	for(int i = 0; i < sensors_min.size(); i++)
	{
		sensors_min[i].sim = sim;
	}
}


void SYSTEM::assign_bag(BAG b)
{
	// Assigns the SYSTEM object's bag field to a user-defined BAG object
	// 
	// Inputs:
	// 		b - [BAG] User-defined bag to estimate
	// 

	bag = b;

	if(sim == true) // If software is running in simulation mode
	{
		for(int i = 0; i < bag.markers.size(); i++) // For each bag marker
		{
			bag.markers[i].position_true = b.markers[i].position; // Assign position_true
		}
	}

	set_sim_flag(sim); // Propagate the sim flag
}


void SYSTEM::calibrate(BAG cal)
{
	// Utilizes an Extended Kalman Filter (EKF) to calibrate the sensor positions for each sensor in the network
	// 
	// Inputs:
	// 		cal - [BAG] Series of markers with known locations used to calibrate the sensor positions
	// 

	cout << "Calibrating Sensors..." << endl;;

	get_data(); // Read the sensor data

	for(int i = 0; i < cnst.m; i++) // For each sensor in the network
	{
		cout << "\tCalibrating Sensor " << i << "...\t\t";

		sensors[i].calibrate_sensor(Y[i], cal, core, cnst); // Calibrate Sensor SID

		if(plot == true) // If the plot flag is set to true
		{
			sensors[i].plot_ekf(); // Plot the EKF results
		}

		cout << "DONE" << endl;
	}

	set_sensors_min(); // Assign the minimal sensor object
}


void SYSTEM::get_data()
{
	// Gets sensor data and organizes the data into a multidimensional vector.
	// Function reads the data from files with the name: './data/Y_sim[sid].txt'
	// 

	vector<vector<DATA>> Y_sid;

	string filename;

	for(int i = 0; i < cnst.m; i++) // For each sensor in the network
	{
		filename = "../data/Y_sim_.txt";
		filename[13] = to_string(i+1)[0]; // Set the SID in the filename

		Y_sid = read_data(filename); // Read the data file
		Y.push_back(Y_sid); // Add the data vector to the multidimensional data vector
	}
}


std::vector<std::vector<DATA>> SYSTEM::read_data(string filename)
{
	// Reads the data file and extracts the data to a multidimensional vector
	// 
	// Inputs:
	// 		filename - [string] Path and filename to the data file
	// 
	// Outputs:
	// 		Y_sid - [vector<vector<DATA>>] Vector containing data for all time k for each marker MID
	// 

	std::vector<std::vector<DATA>> Y_sid;
	std::vector<DATA> Y_k;

	std::ifstream file(filename);

	string str;
	string substr;
	int cnt;
	// int i = 0;
	DATA y_k;

	vector<int> mids; // Vector containing the Marker IDs of measured markers at time k
	vector<int> mid_meas; // Vector containing the number of times a Marker ID has been measured at time k

	while(std::getline(file, str)) // Read each line of the data file
	{
		stringstream part(str);

		cnt = 0;

		bool clr = false;

		while(part.good()) 
		{
			cnt++;

			getline(part, substr, ','); // Read each comma-separated field of the line

			// Assign correct data fields
			if(cnt == 1) // x measurement
			{
				y_k.x = stod(substr);
			}
			else if(cnt == 2) // y measurement
			{
				y_k.y = stod(substr);
			}
			else if(cnt == 3) // Marker ID mid
			{
				y_k.mid = stoi(substr) - 1;

				// This next part of the function determines how many times each marker has been measured at time k
				// If this measurement is the first measurement of marker mid, continue adding data for time k
				// If this measurement is the second measurement of marker mid, push the data (not including this second measurement)
				// and then move to time k+1
				// This prevents multiple measurements of the same marker from being used at the same time k

				bool found = false;
				int i_found;

				for(int i = 0; i < mids.size(); i++) // Loop through the marker IDs that have been measured
				{
					if(mids[i] == y_k.mid) // If the current measurement marker ID is there, flag it
					{
						found = true;
						i_found = i;
					}
				}

				if(found == false) // If the marker ID was not found, add the marker ID to mids
				{
					mids.push_back(y_k.mid);
					mid_meas.push_back(1); // Record that the marker ID has been measured once
				}
				else // If the marker ID was found, increment the measurement count for that marker
				{
					mid_meas[i_found] = mid_meas[i_found] + 1;
					if(mid_meas[i_found] == 2) // If this is the second measurement of marker MID, flag it
					{
						clr =  true;
					}
				}
			}
			else if(cnt == 4) // Sensor ID sid
			{
				y_k.sid = stoi(substr) - 1;
			}
		}

		if(clr == true) // If any marker has been measured twice
		{
			Y_sid.push_back(Y_k); // Add the data for time k
			Y_k.clear(); // Clear the time k measurement vector

			for(int i = 0; i < mid_meas.size(); i++) // Loop through the measurement counts
			{
				if(mid_meas[i] > 0) // Decrement the count if it is not equal to zero
				{
					mid_meas[i] = mid_meas[i] - 1;
				}
			}
		}

		// Add DATA measurement to vectors here
		Y_k.push_back(y_k);
	}

	return Y_sid;
}


void SYSTEM::run_estimator()
{
	// Uses Pixy sensor data to run an Extended Kalman Filter (EKF) for each marker that contains measurements
	// 

	cout << "Running Position Estimator..." << endl;

	vector<vector<vector<DATA>>> Y_reorg;

	Y_reorg = reorg_data(); // Reorganize the data vector

	for(int mid = 0; mid < Y_reorg.size(); mid++)
	{
		if(Y_reorg[mid].size() > 0)
		{
			cout << "\tEstimating Marker " << mid << "... ";

			bag.markers[mid].estimate_pos(Y_reorg[mid], sensors_min, core, cnst); // Estimate the position of Marker MID

			if(plot == true) // If the plot flag is set to true
			{
				bag.markers[mid].plot_ekf(); 
			}

			cout << "DONE" << endl;
		}
	}

	cout << "Running Orientation Estimator..." << endl;

	bag.estimate_ori();

	cout << "DONE" << endl;
}


void SYSTEM::set_sensors_min()
{
	// Assign the properties of the minimal sensor network
	// The SENSOR_MIN class is used for bag estimation after calibration has been completed
	// Objects contain only the necessary data for bag estimation
	// 

	SENSOR_MIN s_min;

	for(int i = 0; i < sensors.size(); i++) // For each sensor defined in the network
	{
		s_min.position = sensors[i].position;
		s_min.point = sensors[i].point;
		s_min.frame = sensors[i].frame;
		s_min.Q_C = sensors[i].Q_C;
		s_min.R = sensors[i].R;
		s_min.dT = sensors[i].dT;
		s_min.sid = sensors[i].sid;
		s_min.sim = sensors[i].sim;

		sensors_min.push_back(s_min);
	}
}


vector<vector<vector<DATA>>> SYSTEM::reorg_data()
{
	// Function to reorganize the data vector Y from Y[sid][k][mid] to Y[mid][k][sid]
	// 
	// Output:
	// 		Y_reorg - [vector<vector<vector<DATA>>>] Reorganized data vector
	// 

	vector<vector<vector<DATA>>> Y_reorg;
	vector<vector<DATA>> Y_k;
	vector<DATA> Y_sid;

	for(int mid = 0; mid < bag.markers.size(); mid++) // Loop through all possible marker IDs
	{
		for(int k = 0; k < Y[0].size(); k++) // Loop through all time steps k
		{
			for(int sid = 0; sid < Y.size(); sid++) // Loop through all sensors
			{
				for(int m = 0; m < Y[sid][k].size(); m++) // Loop through all DATA objects
				{
					if(Y[sid][k][m].mid == mid) // If the measurement is for Marker mid, add it to the vector
					{
						Y_sid.push_back(Y[sid][k][m]);
						break;
					}
				}
			}
			if(Y_sid.size() > 0) // Only add measurements if there are measurements to add
			{
				Y_k.push_back(Y_sid); // Add all measurements of mid at time k
				Y_sid.clear(); // Clear the vector for the next time k
			}
		}
		Y_reorg.push_back(Y_k); // Add all measurements of mid for all time k
		Y_k.clear(); // Clear the vector for the next marker ID
	}

	return Y_reorg;
}