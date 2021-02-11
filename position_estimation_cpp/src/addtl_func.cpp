// Document containing additional functions for position estimation
#include "DATA.hpp"
#include "EKF.hpp"
#include "matplotlibcpp.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
namespace plt = matplotlibcpp;


std::vector<double> get_time_vector(double t_start, double t_stop, double dT)
{
	vector<double> t;

	int steps = (t_stop - t_start)/dT;

	t.push_back(t_start);

	for(int i = 0; i < steps; i++)
	{
		double t_k = t[i] + dT;
		t.push_back(t_k);
	}

	return t;
}

void print_data(std::vector<std::vector<std::vector<DATA>>> Y)
{
	// Print simulated data
	// cout << "Y.size = " << Y.size() << endl;
	// cout << "Y[0].size = " << Y[0].size() << endl;
	// cout << "Y[0][0].size = " << Y[0][0].size() << endl;

	std::vector<double> Yx_plt;

	for(int sid = 0; sid < Y.size(); sid++)
	{
		// std::cout << "sid = " << sid;

		for(int k = 0; k < Y[sid].size(); k++)
		{
			// std::cout << " k = " << k << std::endl;
			for(int mid = 0; mid < Y[sid][k].size(); mid++)
			{
				// std::cout << "    x = " << Y[sid][k][mid].x << "    y = " << Y[sid][k][mid].y << "    mid = " << Y[sid][k][mid].mid << "    sid = " << Y[sid][k][mid].sid << endl;

				if(sid == 0 && mid == 0)
				{
					Yx_plt.push_back(Y[sid][k][mid].x);
				}
			}
		}
		std::cout << std::endl;
	}

	// Plot simulated data
	plt::plot(Yx_plt, "o-");
	plt::show();

}


std::vector<std::vector<DATA>> read_data(string filename)
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