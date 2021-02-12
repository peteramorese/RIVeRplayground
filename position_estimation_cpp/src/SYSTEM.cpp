#include "SYSTEM.hpp"

#include <armadillo>
using namespace arma;
using namespace std;


SYSTEM::SYSTEM(bool s)
{
	cout << "Initializing System...\t\t";

	sim = s; // Set the software run mode

	init_default_sensors();

	cout << "DONE" << endl;
};


SYSTEM::~SYSTEM(){};


void SYSTEM::init_default_sensors()
{
	SENSOR S;

	for(int i = 0; i < cnst.m; i++)
	{
		sensors.push_back(S);
		sensors[i].sim = sim;
		sensors[i].init_default_sensor(i, core);
	}
}


void SYSTEM::calibrate(BAG cal)
{
	cout << "Calibrating Sensors..." << endl;;

	get_data(); // Read the sensor data

	for(int i = 0; i < cnst.m; i++)
	{
		cout << "\tCalibrating Sensor " << i << "...\t\t";

		sensors[i].calibrate_sensor(Y[i], cal, core, cnst);
		sensors[i].plot_ekf();
		cout << "DONE" << endl;
	}

	set_sensors_min();
}


void SYSTEM::get_data()
{
	vector<vector<DATA>> Y_sid;

	string filename;

	for(int i = 0; i < cnst.m; i++)
	{
		filename = "../data/Y_sim_.txt";
		filename[13] = to_string(i+1)[0];

		Y_sid = read_data(filename);
		Y.push_back(Y_sid);
	}
}


std::vector<std::vector<DATA>> SYSTEM::read_data(string filename)
{
	std::vector<std::vector<DATA>> Y_sid;
	std::vector<DATA> Y_k;

	std::ifstream file(filename);

	string str;
	string substr;
	int cnt;
	int i = 0;
	DATA y_k;

	while(std::getline(file, str))
	{
		stringstream part(str);

		cnt = 0;

		while(part.good())
		{
			cnt++;

			getline(part, substr, ',');

			if(cnt == 1)
			{
				y_k.x = stod(substr);
			}
			else if(cnt == 2)
			{
				y_k.y = stod(substr);
			}
			else if(cnt == 3)
			{
				y_k.mid = stoi(substr) - 1;
			}
			else if(cnt == 4)
			{
				y_k.sid = stoi(substr) - 1;
			}
		}

		if((y_k.mid == 1 && y_k.sid == 0) || (y_k.mid == 0 && y_k.sid == 1))
		{
			y_k.x = NAN;
			y_k.y = NAN;
		}

		// Add DATA measurement to vectors here
		Y_k.push_back(y_k);

		if(y_k.mid == 2)
		{
			Y_sid.push_back(Y_k);
			Y_k.clear();
		}
		i++;
	}

	return Y_sid;
}


void SYSTEM::run_estimator()
{
	cout << "Running Estimator... ";

	vector<vector<vector<DATA>>> Y_reorg;

	Y_reorg = reorg_data(); // Reorganize the data vector

	cout << endl << Y_reorg.size() << "  " << Y_reorg[3].size() << "  " << Y_reorg[0][0].size() << endl;

	for(int mid = 0; mid < Y_reorg.size(); mid++)
	{
		if(Y_reorg[mid].size() > 0)
		{
			cout << "Measurements found! " << Y_reorg[mid].size() << endl;
			bag.markers[mid].estimate_pos(Y_reorg[mid], sensors_min, core, cnst);
		}
		else
		{
			cout << "No measurements found :(" << endl;
		}
	}


}


void SYSTEM::set_sensors_min()
{
	SENSOR_MIN s_min;

	for(int i = 0; i < sensors.size(); i++)
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

	cout << "sensors min " << sensors_min.size() << endl;
}


vector<vector<vector<DATA>>> SYSTEM::reorg_data()
{
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

	// for(int i = 0; i < Y_reorg.size(); i++)
	// {
	// 	for(int j = 0; j < Y_reorg[i].size(); j++)
	// 	{
	// 		for(int k = 0; k < Y_reorg[i][j].size(); k++)
	// 		{
	// 			cout << "\tY[" << i << "][" << j << "][" << k << "]\t\t" << Y_reorg[i][j][k].x;
	// 		}
	// 		cout << endl;
	// 	}
	// }


	return Y_reorg;
}