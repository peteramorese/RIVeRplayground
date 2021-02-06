#include<iostream>
#include<vector>
#include<utility>
#include<cmath>
#include "oriDetermine.h"
#include "matrix.h"


std::vector<std::pair<float[3], int>> bag;
std::vector<std::pair<float[3], int>> coords_3Ddata;
bool has_data;
int Niter;
float alpha;

oriDetermine::oriDetermine(std::vector<std::pair<float[3], int>>& bag_){
	bag.resize(bag_.size());
	for (int i=0; i<bag_.size(); i++){
		for (int ii=0; ii<3; ii++){
			bag[i].first[ii] = bag_[i].first[ii];
		}
		bag[i].second = bag_[i].second;
	}	
	has_data = false;
	Niter = 50;
	alpha = .04;
}

void oriDetermine::setData(std::vector<std::pair<float[3], int>>& coords_3Ddata_){
	if (has_data){
		coords_3Ddata.clear();
	}
	coords_3Ddata.resize(coords_3Ddata_.size());
	for (int i=0; i<coords_3Ddata_.size(); i++){
		for (int ii=0; ii<3; ii++){
			coords_3Ddata[i].first[ii] = coords_3Ddata_[i].first[ii];
		}
		coords_3Ddata[i].second = coords_3Ddata_[i].second;
	}	
	has_data = true;
}

void oriDetermine::setIterations(int Niter_){
	Niter = Niter_;
}

void oriDetermine::setAlpha(float alpha_){
	alpha = alpha_;
}

float oriDetermine::norm3(float* arr){
	float norm = 0;
	for (int i=0; i<3; i++){
		norm += (arr[i])*(arr[i]);
	}
	norm = std::sqrt(norm);
	return norm;
}

void oriDetermine::cross3(float* A, float* B, float* product){
	product[0] = A[1]*B[2] - B[1]*A[2];
	product[1] = -A[0]*B[2] + B[0]*A[2];
	product[2] = A[0]*B[1] - B[0]*A[1];

}

/*
void oriDetermine::matMult3x3(float A3[][3], float B3[][3], float product[][3]){
	for (int i=0; i<3; i++){
		for (int j=0; j<3; j++){
			float sum = 0;
			for (int ii=0; ii<3; ii++){
				sum += A3[i][ii]*B3[ii][j];
			}	
			product[i][j] = sum;
		}
	}
}
*/

matrix<float> oriDetermine::q2R(float* v, float theta){
	float qr, qi, qj, qk;
	qr = std::cos(theta/2);
	qi = std::sin(theta/2)*v[0];
	qj = std::sin(theta/2)*v[1];
	qk = std::sin(theta/2)*v[2];
	float s = 1/(qr*qr + qi*qi + qj*qj + qk*qk);

	matrix<float> R(3,3);
	R(0,0) = 1 - 2*s*(qj*qj + qk*qk);
	R(0,1) = 2*s*(qi*qj - qk*qr); 
	R(0,2) = 2*s*(qi*qk + qj*qr); 
	R(1,0) = 2*s*(qi*qj + qk*qr); 
	R(1,1) = 1 - 2*s*(qi*qi + qk*qk); 
	R(1,2) = 2*s*(qj*qk - qi*qr); 
	R(2,0) = 2*s*(qi*qk - qj*qr); 
	R(2,1) = 2*s*(qj*qk + qi*qr); 
	R(2,2) = 1 - 2*s*(qi*qi + qj*qj); 
	return R;
}



matrix<float> oriDetermine::returnQ(){
	if (has_data){
		matrix<float> Q(3,3);
		Q.fill(0.0);
		int data_i = 0;
		std::vector<std::pair<float[3], int>> data_origin;
		std::vector<std::pair<float[3], int>> bag_origin;
		data_origin.resize(coords_3Ddata.size()-1);
		bag_origin.resize(bag.size()-1);
		for (auto& data : coords_3Ddata){
			matrix<float> Q_i(3,3);
			Q_i.fill(1.0f);
			int color = data.second;
			int a = 0;
			for (int i=0; i<coords_3Ddata.size(); i++){
				if (a!=data_i){
					for (int ii=0; ii<3; ii++){
						data_origin[a].first[ii] = coords_3Ddata[i].first[ii] - data.first[ii];
					}
					data_origin[a].second = data.second;
					a++;
				}
			}	
			a = 0;
			int i_currmark;
			for (int i=0; i<bag.size(); i++){
				if (bag[i].second == color){
					i_currmark = i;
					break;
				}		
			}
			for (int i=0; i<bag.size(); i++){
				if (a!=i_currmark){
					for (int ii=0; ii<3; ii++){
						bag_origin[a].first[ii] = bag[i].first[ii] - bag[i_currmark].first[ii];
					}
					bag_origin[a].second = bag[i_currmark].second;
					a++;
				}
			}
			for (int i=0; i<Niter; i++){
				float T[3] = {0, 0, 0};
				for (auto& datum : data_origin){
					int color2 = datum.second;
					int i_matchmark;
					for (int ii=0; ii<bag_origin.size(); ii++){
						if (bag_origin[i].second == color2){
							i_matchmark = i;
							break;
						}		
					}
					float force[3];
					float product[3] = {0, 0, 0};
					float r[3];
					float norm_r;
					norm_r = norm3(bag_origin[i_matchmark].first);	
					for (int ii=0; ii<3; ii++){
						force[ii] = datum.first[ii] - bag_origin[i_matchmark].first[ii];
						r[ii] = bag_origin[i_matchmark].first[ii]/norm_r;	 

					}
					cross3(r, force, product);
					for (int ii=0; ii<3; ii++){
						T[ii] = T[ii] + product[ii];
					}
				}
				float v[3];
				float norm_T = norm3(T);
				for (int ii=0; ii<3; ii++){
					v[ii] = T[ii]/norm_T;	
				}
				float theta = norm_T*alpha;
				matrix<float> R(3,3);
				R = q2R(v, theta);
				Q_i = R*Q_i;
			}
			Q.scale(data_i);
			Q = Q + Q_i;
			Q.scale(1/(data_i + 1));
			data_i++;
		}	
		return Q;
	} else {
		std::cout<<"Error: Must input data set before calling 'returnQ'"<<std::endl;
	}
}
