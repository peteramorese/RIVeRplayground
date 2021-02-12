#include "SYSTEM.hpp"

#include <armadillo>
using namespace arma;
using namespace std;


SYSTEM::SYSTEM()
{
	cout << "Initializing System...\t\t";

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

	vector<vector<vector<DATA>>> Y;
	vector<vector<DATA>> Y_sid;

	string filename;

	for(int i = 0; i < cnst.m; i++)
	{
		cout << "\tCalibrating Sensor " << i << "...\t\t";

		filename = "../data/Y_sim_.txt";
		filename[13] = to_string(i+1)[0];

		Y_sid = read_data(filename);
		Y.push_back(Y_sid);
	
		sensors[i].calibrate_sensor(Y[i], cal, core, cnst);
		sensors[i].plot_ekf();
		cout << "DONE" << endl;
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


void SYSTEM::run_estimator(vector<vector<vector<DATA>>> Y)
{


}