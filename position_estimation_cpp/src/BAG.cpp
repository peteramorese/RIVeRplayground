#include "BAG.hpp"

#include <armadillo>
#include <cmath>
using namespace arma;


BAG::BAG()
{
	for(int i = 0; i < 6; i++)
	{
		MARKER marker;
		marker.mid = i;
		if(i == 1 || i == 2)
		{
			marker.plate = 1;
		}
		markers.push_back(marker);
	}

	markers[0].position = {0, 0, 0.5*height};
	markers[1].position = {0, 0, -0.5*height};
	markers[2].position = {0.5*width, 0, 0};
	markers[3].position = {-0.5*width, 0, 0};
	markers[4].position = {0, 0.5*length, 0};
	markers[5].position = {0, -0.5*length, 0};

	move_bag(zeros<vec>(3), 0, 0, 0);

};

BAG::BAG(int nm)
{
	for(int i = 0; i < nm; i++)
	{
		MARKER marker;
		marker.mid = i;
		markers.push_back(marker);
	}
}

BAG::~BAG(){};

mat BAG::euler_angle_to_dcm(double r, double p, double y)
{
	// Calculates the DCM corresponding to the 3-1-3 Euler Angle rotation
	//
	// Inputs:
	//		double r - [deg] Roll Euler Angle
	//		double p - [deg] Pitch Euler Angle
	// 		double y - [deg] Yaw Euler Angle
	//
	// Outputs:
	// 		mat Q - [3x3] Direction Cosine Matrix corresponding to the 3-1-3 rotation

	r = r * M_PI / 180.0;
	p = p * M_PI / 180.0;
	y = y * M_PI / 180.0;

	mat Q(3, 3);

	Q = {{cos(p), sin(p)*sin(y), cos(y)*sin(p)},
		{sin(r)*sin(p), cos(r)*cos(y) - cos(p)*sin(r)*sin(y), -cos(r)*sin(y) - cos(p)*cos(y)*sin(r)},
		{-cos(r)*sin(p), cos(y)*sin(r) + cos(r)*cos(p)*sin(y), cos(r)*cos(p)*cos(y) - sin(r)*sin(y)}};

	return Q;
};

void BAG::move_bag(vec x, double r, double p, double y)
{
	// Function to move the center of the bag and rotate the bag using 3-1-3 Euler Angle definitions
	// 
	// Inputs:
	//		vec x - [3x1] [m] New position vector of the center of the bag
	//		double r - [deg] Roll Euler Angle
	//		double p - [deg] Pitch Euler Angle
	// 		double y - [deg] Yaw Euler Angle

	center = x; // Reassign the center position of the bag

	mat Q = euler_angle_to_dcm(r, p, y); // Get the DCM for the rotation

	for(int i = 0; i < markers.size(); i++) // For each marker
	{
		markers[i].position = Q * markers[i].position;
		markers[i].position = markers[i].position + center;
	}
};


void BAG::estimate_ori()
{
	std::vector<std::pair<double[3], int>> bag_pair = get_bag_pair();

	oriDetermine od(bag_pair);

	std::vector<std::pair<double[3], int>> coords_3D = get_coords_3D();

	od.setData(coords_3D);
	matrix<double> matout(3, 3);
	matout = od.returnQ();
	matout.print();
}


std::vector<std::pair<double[3], int>> BAG::get_bag_pair()
{
	std::vector<std::pair<double[3], int>> bag_pair;
	int Nbagdef = 6;

	bag_pair.resize(Nbagdef);
	bag_pair[0].first[0] = 0;
	bag_pair[0].first[1] = 0;
	bag_pair[0].first[2] = 0.5*height;
	bag_pair[0].second = 0;
	bag_pair[1].first[0] = 0;
	bag_pair[1].first[1] = 0;
	bag_pair[1].first[2] = -0.5*height;
	bag_pair[1].second = 1;
	bag_pair[2].first[0] = 0.5*width;
	bag_pair[2].first[1] = 0;
	bag_pair[2].first[2] = 0;
	bag_pair[2].second = 2;
	bag_pair[3].first[0] = -0.5*width;
	bag_pair[3].first[1] = 0;
	bag_pair[3].first[2] = 0;
	bag_pair[3].second = 3;
	bag_pair[4].first[0] = 0;
	bag_pair[4].first[1] = 0.5*length;
	bag_pair[4].first[2] = 0;
	bag_pair[4].second = 4;
	bag_pair[5].first[0] = 0;
	bag_pair[5].first[1] = -0.5*length;
	bag_pair[5].first[2] = 0;
	bag_pair[5].second = 5;

	return bag_pair;
}


std::vector<std::pair<double[3], int>> BAG::get_coords_3D()
{
	std::vector<std::pair<double[3], int>> coords_3D;
	int Nmark = 0;

	for(int i = 0; i < markers.size(); i++)
	{
		if(markers[i].updated == true)
		{
			Nmark++;
		}
	}

	coords_3D.resize(Nmark);

	for(int i = 0; i < Nmark; i++)
	{
		coords_3D[i].first[0] = markers[i].position[0];
		coords_3D[i].first[1] = markers[i].position[1];
		coords_3D[i].first[2] = markers[i].position[2];
		coords_3D[i].second = markers[i].mid;
	}

	return coords_3D;
}