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
	// for(int i = 0; i < 5; i++)
	{

		// getline(file, str);

		stringstream part(str);

		cnt = 0;

		while(part.good())
		{
			cnt++;

			getline(part, substr, ',');

			if(cnt == 1)
			{
				y_k.x = stod(substr);
				// cout << "x = " << y_k.x << endl;
			}
			else if(cnt == 2)
			{
				y_k.y = stod(substr);
				// cout << "y = " << y_k.y << endl;
			}
			else if(cnt == 3)
			{
				y_k.mid = stoi(substr) - 1;
				// cout << "mid = " << y_k.mid << endl;
			}
			else if(cnt == 4)
			{
				y_k.sid = stoi(substr) - 1;
				// cout << "sid = " << y_k.sid << endl;
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
		// cout << "i = " << i << endl;
	}

	return Y_sid;
}