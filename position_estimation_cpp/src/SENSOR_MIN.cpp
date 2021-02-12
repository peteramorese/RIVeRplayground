#include "SENSOR_MIN.hpp"

#include <vector>
#include <armadillo>
using namespace arma;


SENSOR_MIN::SENSOR_MIN()
{
	position = zeros<vec>(3);
	point = zeros<vec>(3);
	frame = zeros<mat>(3, 3);
	Q_C = zeros<mat>(3, 3);

	set_params();
}


SENSOR_MIN::SENSOR_MIN(vec pos, vec pnt)
{
	position = pos;
	point = pnt;
	CORE c;
	frame = get_sensor_frame(pos, pnt, c);
	Q_C = c.frame * frame;

	set_params();
}


SENSOR_MIN::~SENSOR_MIN(){};


mat SENSOR_MIN::get_sensor_frame(vec pos, vec pnt, CORE c)
{
	vec z = pnt - pos;
	z = normalise(z);

	vec x = cross(z, c.frame.col(2));
	x = normalise(x);

	vec y = cross(z, x);

	mat S(3, 3);

	S.col(0) = x;
	S.col(1) = y;
	S.col(2) = z;

	return S;
}


void SENSOR_MIN::set_params()
{
	R = 8*eye(2, 2);
	dT = 1.0/60.0;
}
