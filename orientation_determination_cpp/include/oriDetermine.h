#pragma once
#include<vector>
#include<utility>
#include "matrix.h"

class oriDetermine {
	private:
		float norm3(float* arr_ptr);
		void cross3(float* A, float* B, float* product_ptr);
		matrix<float> q2R(float* v, float theta);
	public:
		oriDetermine(std::vector<std::pair<float[3], int>>& bag_);
		void setData(std::vector<std::pair<float[3], int>>& coords_3Ddata_);
		void setIterations(int Niter_);
		void setAlpha(float alpha_);
		matrix<float> returnQ();


};
