#include "MARKER.hpp"

#include "EKF.hpp"

#include <armadillo>
using namespace arma;
using namespace std;


MARKER::MARKER()
{
	position = zeros<vec>(3);
	mid = 0;
	plate = 0;

	init_ekf();
}


MARKER::MARKER(vec p, int m, int pl)
{
	position = p;
	mid = m;
	plate = pl;

	init_ekf();
}


MARKER::~MARKER(){};


void MARKER::init_ekf()
{
	EKF e;
	e.x_hat = zeros(3, 1);
	e.P = {{pow(5, 2), 0, 0},
			{0, pow(1.195, 2), 0},
			{0, 0, pow(1.195, 2)}};
}


void MARKER::estimate_pos(vector<vector<vector<DATA>>> Y, CORE core, CONSTANTS cnst)
{

}