#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cmath>
#include "oriDetermine.h"
#include "matrix.h"

int main() {
	

	// Insert Bag Data
	std::vector<std::pair<double[3], int>> bag;
	int Nbagdef = 6;
	double width = 4;
	double height = 1.6;
	double depth = 2.4;
	double rbag[3] = {5, 0, 0};
	bag.resize(Nbagdef);
	bag[0].first[0] = depth/2;
	bag[0].first[1] = 0; 
	bag[0].first[2] = 0;
	bag[0].second = 1;
	bag[1].first[0] = 0;
	bag[1].first[1] = width/2; 
	bag[1].first[2] = 0;
	bag[1].second = 2;
	bag[2].first[0] = 0; 
	bag[2].first[1] = 0; 
	bag[2].first[2] = height/2;
	bag[2].second = 3;
	bag[3].first[0] = -depth/2;
	bag[3].first[1] = 0; 
	bag[3].first[2] = 0;
	bag[3].second = 4;
	bag[4].first[0] = 0;
	bag[4].first[1] = -width/2; 
	bag[4].first[2] = 0;
	bag[4].second = 5;
	bag[5].first[0] = 0;
	bag[5].first[1] = 0; 
	bag[5].first[2] = -height/2;
	bag[5].second = 6;

	oriDetermine od(bag);	
	std::vector<std::pair<double[3], int>> coords_3D;
	coords_3D.resize(4);
	coords_3D[0].first[0] = 5.3616586624;
	coords_3D[0].first[1] = 1.0104812331;
	coords_3D[0].first[2] = -0.0375358438;
	coords_3D[0].second = 1;
	/*
	coords_3D[1].first[0] =	4.4653518626;
	coords_3D[1].first[1] = 0.0717397401;
	coords_3D[1].first[2] = -1.9592845933;
	coords_3D[1].second = 2;
	*/
	coords_3D[1].first[0] = 4.3231051188;
	coords_3D[1].first[1] = 0.1058191446;
	coords_3D[1].first[2] = 0.1957314767;
	coords_3D[1].second = 3;
	coords_3D[2].first[0] = 4.6374827331;
	coords_3D[2].first[1] = -1.2470335490;
	coords_3D[2].first[2] = -0.0155806898;
	coords_3D[2].second = 4;
	/*
	coords_3D[4].first[0] = 5.5099273425;
	coords_3D[4].first[1] = -0.2650047536;
	coords_3D[4].first[2] = 1.8744393847;
	coords_3D[4].second = 5;
	*/
	coords_3D[3].first[0] = 5.7227776127;
	coords_3D[3].first[1] = -0.2848180723;
	coords_3D[3].first[2] = -0.2852411704;
	coords_3D[3].second = 6;

	od.setData(coords_3D);
	od.setIterations(1000);
	matrix<double> matout(3,3);
	matout = od.returnQ();
	matout.print();
	

	//play around with matrixClass to make sure it works
	/*	

	std::cout<<"\n";
	std::cout<<"\n";
	std::cout<<"\n";
	matrix<double> mat1(3,3);
	mat1(0,0) = 1;
	mat1(0,1) = 2;
	mat1(0,2) = 3;
	mat1(1,0) = 4;
	mat1(1,1) = 5;
	mat1(1,2) = 6;
	mat1(2,0) = 7;
	mat1(2,1) = 8;
	mat1(2,2) = 9;
	mat1.print();
	mat1.scale(1);
	matrix<double> mat2(3,3);
	std::cout<<"\n";
	mat2 = mat1 + mat1;
	mat2.print();
	std::cout<<"\n";
	*/


	return 0;

}
