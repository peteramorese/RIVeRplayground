#include<iostream>
#include<vector>
#include<cmath>
#include "oriDetermine.h"
#include "matrix.h"

int main() {

	// Insert Bag Data
	std::vector<std::pair<float[3], int>> bag;
	int Nbagdef = 6;
	float width = 4;
	float height = 1.6;
	float depth = 2.4;
	float rbag[3] = {5, 0, 0};
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
	//od.returnQ(Q);

	//play around with matrixClass to make sure it works
	matrix<int> mat1(2,2);
	mat1.fill(0);
	mat1(0,0) = 1;
	mat1(0,1) = 2;
	mat1(1,0) = 3;
	mat1(1,1) = 4;
	std::cout<< "mat1: \n";
	mat1.print();
	std::cout<<"\n";
	matrix<int> mat2(2,2);
	mat2(0,0) = 4;
	mat2(0,1) = 6;
	mat2(1,0) = 9;
	mat2(1,1) = 1;
	std::cout<< "mat2: \n";
	mat2.print();
	mat2.scale(100);
	std::cout<<"\n";
	matrix<int> mat3(3,4);
	mat2 = mat1-mat2;
	std::cout<< "mat2: \n";
	mat2.print();
	std::cout<<"\n";


	return 0;

}
