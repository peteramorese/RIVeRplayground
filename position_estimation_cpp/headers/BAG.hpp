#ifndef BAG_H
#define BAG_H

#include "MARKER.hpp"

#include <armadillo>
using namespace arma;


// Class defining a cargo bag
class BAG
{
public:
	vec center;
	double length = 0.41; // [m] Cargo bag length
	double width = 0.23; // [m] Cargo bag width
	double height = 0.24; // [m] Cargo bag height
	std::vector<MARKER> markers;

	BAG();
	BAG(int nm);
	~BAG();
	void move_bag(vec x, double roll, double pitch, double yaw);
	mat euler_angle_to_dcm(double roll, double pitch, double yaw);

};

#endif /* BAG_H */