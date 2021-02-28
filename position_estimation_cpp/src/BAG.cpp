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
	std::vector<std::pair<double[3], int>> bag_pair = get_bag_pair(true);

	oriDetermine od(bag_pair);

	std::vector<std::pair<double[3], int>> coords_3D = get_coords_3D();

	od.setData(coords_3D);
	od.setIterations(10000);
	matrix<double> matout(3, 3);
	matout = od.returnQ();
	matout.print();

	mat Q;
	vec curr;

	Q = {{matout(0, 0), matout(0, 1), matout(0, 2)},
		{matout(1, 0), matout(1, 1), matout(1, 2)}, 
		{matout(2, 0), matout(2, 1), matout(2, 2)}};

	std::vector<std::pair<double[3], int>> bag_def = get_bag_pair(true);

	// Compute the rotated (and centered) positions of each marker
	for(int i = 0; i < bag_def.size(); i++)
	{
		curr = {bag_def[i].first[0], bag_def[i].first[1], bag_def[i].first[2]};
		curr = Q * curr;

		bag_def[i].first[0] = curr[0];
		bag_def[i].first[1] = curr[1];
		bag_def[i].first[2] = curr[2];
	}

	// Find the first estimated marker index
	int Index;

	// markers[0].updated = true;

	for(int i = 0; i < markers.size(); i++)
	{
		if(markers[i].updated == true)
		{
			Index = i;
			cout << "Index " << i << endl;
			break;
		}
	}

	vec move;
	move = {markers[Index].position[0] - bag_def[Index].first[0], 
		markers[Index].position[1] - bag_def[Index].first[1],
		markers[Index].position[2] - bag_def[Index].first[2]};

	for(int i = 0; i < bag_def.size(); i++)
	{
		if(markers[i].updated == false)
		{
			curr = {bag_def[i].first[0], bag_def[i].first[1], bag_def[i].first[2]};

			markers[i].position = curr + move;
		}

		cout << "M" << i << "  " << markers[i].position[0] << "  " << markers[i].position[1] << "  " << markers[i].position[2] << endl;
	}
}

// void BAG::estimate_ori()
// {
// 	for(int i = 0; i < 6; i++)
// 	{
// 		markers[i].updated = false;
// 		markers[i].position = {0, 0, 0};
// 	}

// 	std::vector<std::pair<double[3], int>> bag_pair = get_bag_pair(true);

// 	oriDetermine od(bag_pair);

// 	std::vector<std::pair<double[3], int>> coords_3D = get_coords_3D();

// 	od.setData(coords_3D);
// 	od.setIterations(1000);
// 	matrix<double> matout(3, 3);
// 	matout = od.returnQ();
// 	matout.print();

// 	mat Q;
// 	vec curr;

// 	Q = {{matout(0, 0), matout(0, 1), matout(0, 2)},
// 		{matout(1, 0), matout(1, 1), matout(1, 2)}, 
// 		{matout(2, 0), matout(2, 1), matout(2, 2)}};
// 	// Q = Q;

// 	std::vector<std::pair<double[3], int>> bag_def = get_bag_pair(true);

// 	// Compute the rotated (and centered) positions of each marker
// 	for(int i = 0; i < bag_def.size(); i++)
// 	{
// 		curr = {bag_def[i].first[0], bag_def[i].first[1], bag_def[i].first[2]};
// 		cout << "M" << i << "Original" << endl;
// 		cout << curr << endl;
// 		curr = Q * curr;
		
// 		cout << "M" << i << endl << curr << endl;
// 		bag_def[i].first[0] = curr[0];
// 		bag_def[i].first[1] = curr[1];
// 		bag_def[i].first[2] = curr[2];
// 	}

// 	// Find the first estimated marker index
// 	int Index;

// 	markers[0].updated = true;
// 	markers[0].position = {0.212, 0.103, -0.258};

// 	for(int i = 0; i < markers.size(); i++)
// 	{
// 		if(markers[i].updated == true)
// 		{
// 			Index = i;
// 			break;
// 		}
// 	}

// 	vec move;
// 	move = {markers[Index].position[0] - bag_def[Index].first[0], 
// 		markers[Index].position[1] - bag_def[Index].first[1],
// 		markers[Index].position[2] - bag_def[Index].first[2]};

// 	for(int i = 0; i < bag_def.size(); i++)
// 	{
// 		if(markers[i].updated == false || true)
// 		{
// 			curr = {bag_def[i].first[0], bag_def[i].first[1], bag_def[i].first[2]};

// 			markers[i].position = curr + move;
// 		}

// 		cout << "M" << i << "  " << markers[i].position[0] << "  " << markers[i].position[1] << "  " << markers[i].position[2] << endl;
// 	}
// }


std::vector<std::pair<double[3], int>> BAG::get_bag_pair(bool def)
{
	std::vector<std::pair<double[3], int>> bag_def;
	std::vector<std::pair<double[3], int>> bag_pair;

	// CHANGE THE BAG DEFINITION FOR THE REAL THING YOU DUMB BITCH
	bag_def.resize(6);
	bag_def[0].first[0] = 0;
	bag_def[0].first[1] = 0;
	bag_def[0].first[2] = 0.5*height;
	bag_def[0].second = 0;
	bag_def[1].first[0] = 0;
	bag_def[1].first[1] = 0;
	bag_def[1].first[2] = -0.5*height;
	bag_def[1].second = 1;
	bag_def[2].first[0] = 0.5*width;
	bag_def[2].first[1] = 0;
	bag_def[2].first[2] = 0;
	bag_def[2].second = 2;
	bag_def[3].first[0] = -0.5*width;
	bag_def[3].first[1] = 0;
	bag_def[3].first[2] = 0;
	bag_def[3].second = 3;
	bag_def[4].first[0] = 0;
	bag_def[4].first[1] = 0.5*height;
	bag_def[4].first[2] = 0;
	bag_def[4].second = 4;
	bag_def[5].first[0] = 0;
	bag_def[5].first[1] = -0.5*height;
	bag_def[5].first[2] = 0;
	bag_def[5].second = 5;

	vector<int> mark;

	for(int i = 0; i < markers.size(); i++)
	{
		if(markers[i].updated == true || def)
		{
			mark.push_back(i);
		}
	}

	for(int i = 0; i < mark.size(); i++)
	{
		bag_pair.push_back(bag_def[mark[i]]);
	}

	return bag_pair;
}

// std::vector<std::pair<double[3], int>> BAG::get_bag_pair(bool def)
// {
// 	std::vector<std::pair<double[3], int>> bag_def;
// 	int Nbagdef = 6;
// 	double rbag[3] = {5, 0, 0};
// 	bag_def.resize(Nbagdef);
// 	// bag[0].first[0] = depth/2;
// 	// bag[0].first[1] = 0; 
// 	// bag[0].first[2] = 0;
// 	// bag[0].second = 1;
// 	// bag[1].first[0] = 0;
// 	// bag[1].first[1] = width/2; 
// 	// bag[1].first[2] = 0;
// 	// bag[1].second = 2;
// 	// bag[2].first[0] = 0; 
// 	// bag[2].first[1] = 0; 
// 	// bag[2].first[2] = height/2;
// 	// bag[2].second = 3;
// 	// bag[3].first[0] = -depth/2;
// 	// bag[3].first[1] = 0; 
// 	// bag[3].first[2] = 0;
// 	// bag[3].second = 4;
// 	// bag[4].first[0] = 0;
// 	// bag[4].first[1] = -width/2; 
// 	// bag[4].first[2] = 0;
// 	// bag[4].second = 5;
// 	// bag[5].first[0] = 0;
// 	// bag[5].first[1] = 0; 
// 	// bag[5].first[2] = -height/2;
// 	// bag[5].second = 6;
// 	bag_def[0].first[0] = 0;
// 	bag_def[0].first[1] = 0;
// 	bag_def[0].first[2] = 0.5*height;
// 	bag_def[0].second = 0;
// 	bag_def[1].first[0] = 0;
// 	bag_def[1].first[1] = 0;
// 	bag_def[1].first[2] = -0.5*height;
// 	bag_def[1].second = 1;
// 	bag_def[2].first[0] = 0.5*width;
// 	bag_def[2].first[1] = 0;
// 	bag_def[2].first[2] = 0;
// 	bag_def[2].second = 2;
// 	bag_def[3].first[0] = -0.5*width;
// 	bag_def[3].first[1] = 0;
// 	bag_def[3].first[2] = 0;
// 	bag_def[3].second = 3;
// 	bag_def[4].first[0] = 0;
// 	bag_def[4].first[1] = 0.5*height;
// 	bag_def[4].first[2] = 0;
// 	bag_def[4].second = 4;
// 	bag_def[5].first[0] = 0;
// 	bag_def[5].first[1] = -0.5*height;
// 	bag_def[5].first[2] = 0;
// 	bag_def[5].second = 5;

// 	return bag_def;
// }


std::vector<std::pair<double[3], int>> BAG::get_coords_3D()
{
	std::vector<std::pair<double[3], int>> coords_3D;
	int Nmark = 0;
	vector<int> mark;

	for(int i = 0; i < markers.size(); i++)
	{
		if(markers[i].updated == true)
		{
			Nmark++;
			mark.push_back(i);
		}
	}

	coords_3D.resize(Nmark);

	for(int i = 0; i < Nmark; i++)
	{
		coords_3D[i].first[0] = markers[mark[i]].position[0];
		coords_3D[i].first[1] = markers[mark[i]].position[1];
		coords_3D[i].first[2] = markers[mark[i]].position[2];
		coords_3D[i].second = markers[mark[i]].mid;

		cout << "i = " << mark[i] << "  " << coords_3D[i].second << endl;
	}

	return coords_3D;
}


// std::vector<std::pair<double[3], int>> BAG::get_coords_3D()
// {
// 	std::vector<std::pair<double[3], int>> coords_3D;
// 	coords_3D.resize(6);
// 	// coords_3D[0].first[0] = 5.3616586624;
// 	// coords_3D[0].first[1] = 1.0104812331;
// 	// coords_3D[0].first[2] = -0.0375358438;
// 	// coords_3D[0].second = 1;
// 	// coords_3D[1].first[0] =	4.4653518626;
// 	// coords_3D[1].first[1] = 0.0717397401;
// 	// coords_3D[1].first[2] = -1.9592845933;
// 	// coords_3D[1].second = 2;
// 	// coords_3D[2].first[0] = 4.3231051188;
// 	// coords_3D[2].first[1] = 0.1058191446;
// 	// coords_3D[2].first[2] = 0.1957314767;
// 	// coords_3D[2].second = 3;
// 	// coords_3D[3].first[0] = 4.6374827331;
// 	// coords_3D[3].first[1] = -1.2470335490;
// 	// coords_3D[3].first[2] = -0.0155806898;
// 	// coords_3D[3].second = 4;
// 	// coords_3D[4].first[0] = 5.5099273425;
// 	// coords_3D[4].first[1] = -0.2650047536;
// 	// coords_3D[4].first[2] = 1.8744393847;
// 	// coords_3D[4].second = 5;
// 	// coords_3D[5].first[0] = 5.7227776127;
// 	// coords_3D[5].first[1] = -0.2848180723;
// 	// coords_3D[5].first[2] = -0.2852411704;
// 	// coords_3D[5].second = 6;

// 	coords_3D[0].first[0] = 0.212;
// 	coords_3D[0].first[1] = 0.103;
// 	coords_3D[0].first[2] = -0.258;
// 	coords_3D[0].second = 0;
// 	coords_3D[1].first[0] =	0.208;
// 	coords_3D[1].first[1] = -0.003;
// 	coords_3D[1].first[2] = -0.042;
// 	coords_3D[1].second = 1;
// 	coords_3D[2].first[0] = 0.322;
// 	coords_3D[2].first[1] = 0.074;
// 	coords_3D[2].first[2] = -0.137;
// 	coords_3D[2].second = 2;
// 	coords_3D[3].first[0] = 0.098;
// 	coords_3D[3].first[1] = 0.026;
// 	coords_3D[3].first[2] = -0.163;
// 	coords_3D[3].second = 3;
// 	coords_3D[4].first[0] = 0.239;
// 	coords_3D[4].first[1] = -0.055;
// 	coords_3D[4].first[2] = -0.201;
// 	coords_3D[4].second = 4;
// 	coords_3D[5].first[0] = 0.181;
// 	coords_3D[5].first[1] = 0.155;
// 	coords_3D[5].first[2] = -0.099;
// 	coords_3D[5].second = 5;

// 	return coords_3D;
// }