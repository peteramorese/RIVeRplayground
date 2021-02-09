Documentation for "oriDetermine.h"
	Author: Peter Amorese
	Purpose: Bag configuration algorithm for use in the RIVeR Senior Project

How to use:

	oriDetermine od(vector<pair<float[3] r_vec, int markerID>>)
		Instantiate class with the definition of the bag in the constructor. 
		Bag must be of form "std::vector<std::pair<float[3], int>>".

	od.setData(vector<pair<float[3] r_vec, int markerID>>)
		Specify the data set to determine. Same data structure as the bag. This
		can be called again with a new dataset before or after the call to returnQ()

	matrix<float> Q = od.returnQ()
		Determine the best fit orientation of the current data

Public member functions:
	oriDetermine(std::vector<std::pair<float[3], int>>& bag_);
		Constructor, specify bag-in-memory definition

	void setData(std::vector<std::pair<float[3], int>>& coords_3Ddata_);
		Set the input 3D data. This must be called atleast once before calling return()

	void setIterations(int Niter_);
		Manually set the number of iterations. If this is not called, the default is 50

	void setAlpha(float alpha_);
		Manually set the gradient gain. If this is not called, the default is .04

	matrix<float> returnQ();
		Determine the best Q matrix to match the input 3D data given the bag definition
		specified in the constructor. Must set data before calling this function

