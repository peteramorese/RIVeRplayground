#include "SENSOR.hpp"

#include "matplotlibcpp.h"

#include <vector>
#include <cmath>
#include <armadillo>
#include <iostream>
#include <string>
#include <sstream>
using namespace arma;
namespace plt = matplotlibcpp;


SENSOR::SENSOR()
{
	position = zeros<vec>(3);
	position_true = zeros<vec>(3);
	point = zeros<vec>(3);
	frame = zeros<mat>(3, 3);
	Q_C = zeros<mat>(3, 3);

	set_params();
}


SENSOR::SENSOR(vec pos, vec pnt)
{
	position = pos;
	position_true = zeros<vec>(3);
	point = pnt;
	CORE c;
	frame = get_sensor_frame(pos, pnt, c);
	Q_C = c.frame * frame;

	set_params();
}


SENSOR::~SENSOR(){};


mat SENSOR::get_sensor_frame(vec pos, vec pnt, CORE c)
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

void SENSOR::set_params()
{
	R = 6*eye(2, 2);
	dT = 1.0/60.0;
}

void SENSOR::init_default_sensor(int id, CORE c)
{
	// Function to initalize the default positions and point of the sensor system
	// Positions and pointing must be defined here for operation
	// Positions will be calibrated more precisely later
	//
	// Inputs:
	//		int id - Sensor ID to initiate
	//		CORE c - Struct defining the Core Frame

	sid = id;

	if(id == 0)
	{
		position = {0.5, -0.55, -0.98};
		point = {0, 0, 0};
		if(sim == true)
		{
			position_true = {0.5888, -0.3422, -0.9577};
		}
	}
	else if(id == 1)
	{
		position = {0.56, 0.52, -0.6};
		point = {0, 0, 0};
		if(sim == true)
		{
			position_true = {0.5230, 0.5525, -0.4486};
		}
	}
	else if(id == 2)
	{
		position = {0.56, 0.52, 0.6};
		point = {0, 0, 0};
		if(sim == true)
		{
			position_true = {0.5365, 0.6307, 0.4397};
		}
	}
	else if(id == 3)
	{

	}
	else if(id == 4)
	{

	}

	// cout << "true = \n" << position_true << endl;
	// cout << "est = \n" << position << endl;

	frame = get_sensor_frame(position, point, c);

	EKF e;
	e.x_hat = position;
	e.P = 2*eye(3, 3);
	if(sim == true)
	{
		e.e_x = position - position_true;
	}

	ekf.push_back(e);
}


void SENSOR::calibrate_sensor(std::vector<std::vector<DATA>> Y, BAG bag, CORE core, CONSTANTS cnst)
{
	// Function to calibrate the sensor's position based on measurements of markers at known positions
	// Uses an Extended Kalman Filter to estimate
	// 
	// Inputs:
	// 		vector<double> t - [s] Time vector
	// 		vector<vector<DATA>> Y - Packets of measurements from the sensor being calibrated of multiple markers for time t
	// 		BAG bag - Object defining the known positions of the markers being measured
	// 		CORE core - Object defining the Core Frame
	// 		CONSTANTS cnst - Object defining system-wide constants
	// 


	EKF e;
	std::vector<DATA> y_k;

	for(int k = 0; k < Y.size(); k++) // For each time k
	{
		y_k = Y[k]; // Measurement of all markers at time k

		e = ekf_update(ekf[ekf.size()-1], bag, y_k, core, cnst); // Get next EKF update

		ekf.push_back(e); // Add EKF update to the sensor
	}
}


EKF SENSOR::ekf_update(EKF e, BAG bag, std::vector<DATA> y_k, CORE core, CONSTANTS cnst)
{
	// Function to perform an Extended Kalman Filter update given the previous EKF results and new measurements
	// 
	// Inputs:
	// 		EKF e - The previous estimation results of the EKF
	// 		vector<DATA> y_k - New measurements from the sensor at time k of each marker
	// 		CORE core - Object defining the Core Frame
	// 
	// Outputs:
	// 		EKF e_upd - Updated EKF step
	// 


	EKF e_kp1;
	int num_meas = 0; // Number of measurements (corresponds to one for each marker)
	std::vector<int> i_valid;

	for(int i = 0; i < y_k.size(); i++)
	{
		if(~isnan(y_k[i].x))
		{
			num_meas++; // Measurement is valid
			i_valid.push_back(i); // Measurement is of Marker MID
		}
	}

	if(num_meas > 0) // Measurement update
	{
		std::tuple<double, double> yhat_tup;
		vec yhat(cnst.p*num_meas);
		vec y(cnst.p*num_meas);
		mat H_tilde;
		mat R_tmp;
		mat R_blk;

		for(int i = 0; i < num_meas; i++)
		{

			yhat_tup = get_yhat(bag.markers[i_valid[i]], core, false); // Get predicted measurement

			yhat[2*i] = std::get<0>(yhat_tup); // Store yhat in a stacked vector
			yhat[2*i+1] = std::get<1>(yhat_tup);

			y[2*i] = y_k[i_valid[i]].x; // Store real measurements in a stacked vector
			y[2*i+1] = y_k[i_valid[i]].y;

			mat H = get_H_tilde(bag.markers[i_valid[i]]); 
			H_tilde = std::move(arma::join_cols(H_tilde, H));

			R_tmp = std::move(arma::join_rows(zeros(2, R_blk.n_cols), R));
			R_blk = std::move(arma::join_rows(R_blk, zeros(R_blk.n_rows, 2)));
			R_blk = std::move(arma::join_cols(R_blk, R_tmp));
		}

		e_kp1.e_y = y - yhat; // Measurement Innovation

		// Kalman gain matrix
		mat K_tilde = e.P * H_tilde.t() * (H_tilde * e.P * H_tilde.t() + R_blk).i();

		e_kp1.x_hat = e.x_hat + K_tilde * e_kp1.e_y; // New state estimate

		mat I = eye(cnst.n, cnst.n); // Identity

		e_kp1.P = (I - K_tilde * H_tilde)*e.P; // New covariance matrix


	}
	else // No measurement update
	{
		e_kp1.x_hat = e.x_hat;
		e_kp1.P = e.P;
	}

	if(sim == true) // If the software is running in simulation mode
	{
		e_kp1.e_x = e_kp1.x_hat - position_true;
	}

	position = e_kp1.x_hat; // Update sensor position
	frame = get_sensor_frame(position, point, core); // Update pointing frame
	Q_C = core.frame * frame; // Update DCM

	return e_kp1;
}


std::tuple<double, double> SENSOR::get_yhat(MARKER m, CORE c, bool sim = false)
{
	// The nonlinear measurement function for yhat
	//
	// Inputs:
	//		MARKER m - Marker being measured
	//		CORE c - Object defining the Core Frame
	//		bool sim - Flag for whether data is simulated or real expected
	//
	// Outputs:
	//		tuple yhat - Pixy output [x, y]


	mat C = c.frame; // Core Frame matrix

	double gamma_rad = gamma * M_PI / 180.0; // [rad] Horizontal FOV half-angle
	double theta_rad = theta * M_PI / 180.0; // [rad] Vertical FOV half-angle

	// Constant matrices / vector
	mat T = {{tan(gamma_rad), 0, 0},
		{0, tan(theta_rad), 0},
		{0, 0, 1}};

	mat DELTA = {{2.0/x_max, 0},
		{0, 2.0/y_max},
		{0, 0}};

	vec ivec = {1.0, 1.0, -1.0};

	vec pos; // Position vector to calculate with

	// Nonlinear measurement function
	if(sim == true) // Use true position
	{
		pos = position_true;
	}
	else // Use estimated position
	{
		pos = position;
	}

	vec V_S = frame.t() * C.t() * (m.position - pos);
	double a = V_S[2];

	mat H = (1.0/a) * (DELTA.t() * DELTA).i() * DELTA.t() * T.i() * frame.t() * C.t();
	mat G = -(1.0/a) * (DELTA.t() * DELTA).i() * DELTA.t() * T.i() * frame.t() * C.t() * pos + (DELTA.t() * DELTA).i() * DELTA.t() * ivec;

	vec yhat_vec = H * m.position + G; // Predicted measurement yhat

	std::tuple<double, double> yhat;

	if(sim == true) // If sim flag to corrupt with measurement noise is set to true
	{
		arma_rng::set_seed_random();
		vec mu = {0, 0};
		mat N = mvnrnd(mu, R); // Generate measurement noise

		yhat_vec = yhat_vec + N; // Corrupt measurements with noise
	}

	if(sim == true) // If sim flag to round measurements is set to true
	{
		yhat_vec = round(yhat_vec); // Round the measurements to the nearest int
	}

	std::get<0>(yhat) = yhat_vec[0]; // Assign measurements to a tuple
	std::get<1>(yhat) = yhat_vec[1];

	if(sim == true) // If sim flag to account for visibility is set to true
	{
		// If marker is outside the visibility of the sensor
		if(std::get<0>(yhat) > x_max || std::get<0>(yhat) < 0 || std::get<1>(yhat) > y_max || std::get<1>(yhat) < 0)
		{
			std::get<0>(yhat) = NAN; // Return NAN
			std::get<1>(yhat) = NAN;
		}
	}

	return yhat;
}


mat SENSOR::get_H_tilde(MARKER m)
{
	// Function to find the H_tilde matrix for calibration of the sensor positioning system
	// 
	// Inputs:
	// 		MARKER m - Object containing the position of the marker being measured
	// 
	// Outputs:
	// 		mat H - H_tilde matrix used in calibration, equal to dh/ds
	// 

	// cout << "x = \n" << position << endl;
	// cout << "m = \n" << m.position << endl;

	double gamma_rad = gamma * M_PI / 180.0; // [rad] Horizontal FOV half-angle
	double theta_rad = theta * M_PI / 180.0; // [rad] Vertical FOV half-angle

	mat T = {{tan(gamma_rad), 0, 0},
			{0, tan(theta_rad), 0},
			{0, 0, 1}};

	mat DELTA = {{2.0/x_max, 0},
			{0, 2.0/y_max},
			{0, 0}};

	double s1 = position[0];
	double s2 = position[1];
	double s3 = position[2];

	double x1 = m.position[0];
	double x2 = m.position[1];
	double x3 = m.position[2];

	// The H_tilde_partial matrix was computed symbolically in Mathematica, and hand-typed here
	mat H_tilde_partial(3, 3, arma::fill::zeros);

	double s_mag = sqrt(pow(s1, 2.0) + pow(s2, 2.0) + pow(s3, 2.0)); // Magnitude of the vector s
	double s_denom = sqrt(pow(s1, 2.0)/pow(s_mag, 2.0) + pow(s2, 2.0)/pow(s_mag, 2)); // Common denominator term in the Jacobian

	H_tilde_partial(0, 0) = -(((-s2*x1/(s_mag*s_denom) + s1*x2/(s_mag*s_denom))*(s1/s_mag + pow(s1, 2.0)*(x1-s1)/pow(s_mag, 3.0) - 
		(x1-s1)/s_mag + s1*s2*(x2-s2)/pow(s_mag, 3.0) + s1*s3*(x3-s3)/pow(s_mag, 3.0)))/pow((-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - 
		s3*(x3-s3)/s_mag), 2.0)) + (s2*(-2*pow(s1, 3.0)/pow(s_mag, 4.0) - 2*s1*pow(s2, 2.0)/pow(s_mag, 4.0) + 
		2*s1/pow(s_mag, 2.0))*x1/(2*s_mag*pow(s_denom, 3.0)) + s1*s2*x1/(pow(s_mag, 3.0)*s_denom) - s1*(-2*pow(s1, 3.0)/pow(s_mag, 4.0) - 
		2*s1*pow(s2, 2.0)/pow(s_mag, 4.0) + 2*s1/pow(s_mag, 2))*x2/(2*s_mag*pow(s_denom, 3.0)) - pow(s1, 2.0)*x2/(pow(s_mag, 3.0)*s_denom) + 
		x2/(s_mag*s_denom))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);

	H_tilde_partial(0, 1) = -(((-s2*x1/(s_mag*s_denom) + s1*x2/(s_mag*s_denom))*(s2/s_mag + s1*s2*(x1-s1)/(pow(s_mag, 3.0)) + 
		pow(s2, 2.0)*(x2-s2)/pow(s_mag, 3.0) - (x2-s2)/s_mag + s2*s3*(x3-s3)/pow(s_mag, 3.0)))/pow((-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - 
		s3*(x3-s3)/s_mag), 2.0)) + (s2*(-2*pow(s1, 2.0)*s2/pow(s_mag, 4.0) - 2*pow(s2, 3.0)/pow(s_mag, 4.0) + 
		2*s2/pow(s_mag, 2.0))*x1/(2*s_mag*pow(s_denom, 3.0)) + pow(s2, 2.0)*x1/(pow(s_mag, 3.0)*s_denom) - x1/(s_mag*s_denom) - 
		s1*(-2*pow(s1, 2.0)*s2/pow(s_mag, 4.0) - 2*pow(s2, 3.0)/pow(s_mag, 4.0) + 2*s2/pow(s_mag, 2.0))*x2/(2*s_mag*pow(s_denom, 3.0)) - 
		s1*s2*x2/(pow(s_mag, 3.0)*s_denom))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);

	H_tilde_partial(0, 2) = -(((-s2*x1/(s_mag*s_denom) + s1*x2/(s_mag*s_denom))*(s3/s_mag + s1*s3*(x1-s1)/(pow(s_mag, 3.0)) + 
		s2*s3*(x2-s2)/pow(s_mag, 3.0) + pow(s3, 2.0)*(x3-s3)/pow(s_mag, 3.0) - (x3-s3)/s_mag))/pow((-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - 
		s3*(x3-s3)/s_mag), 2.0)) + (s2*(-2*pow(s1, 2.0)*s3/pow(s_mag, 4.0) - 2*pow(s2, .0)*s3/pow(s_mag, 4.0))*x1/(2*s_mag*pow(s_denom, 3.0)) + 
		s2*s3*x1/(pow(s_mag, 3.0)*s_denom) - s1*(-2*pow(s1, 2.0)*s3/pow(s_mag, 4.0) - 2*pow(s2, 2.0)*s3/pow(s_mag, 4.0))*x2/(2*s_mag*pow(s_denom, 3.0)) - 
		s1*s3*x2/(pow(s_mag, 3.0)*s_denom))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);

	H_tilde_partial(1, 0) = -(((-pow(s1, 2.0)*s3/(pow(s_mag, 2.0)*s_denom) - pow(s2, 2.0)*s3/(pow(s_mag, 2.0)*s_denom) - 
		s3*(-pow(s1, 2.0)/(pow(s_mag, 2.0)*s_denom) - pow(s2, 2.0)/(pow(s_mag, 2.0)*s_denom)) + s1*s3*x1/(pow(s_mag, 2.0)*s_denom) + 
		s2*s3*x2/(pow(s_mag, 2.0)*s_denom) + (-pow(s1, 2.0)/(pow(s_mag, 2.0)*s_denom) - pow(s2, 2.0)/(pow(s_mag, 2.0)*s_denom))*x3)*(s1/s_mag + 
		pow(s1, 2.0)*(x1-s1)/pow(s_mag, 3.0) - (x1-s1)/s_mag + s1*s2*(x2-s2)/pow(s_mag, 3.0) + s1*s3*(x3-s3)/pow(s_mag, 3.0)))/pow((-s1*(x1-s1)/s_mag - 
		s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag), 2.0)) + (pow(s1, 2.0)*s3*(-2*pow(s1, 3.0)/pow(s_mag, 4.0) - 2*s1*pow(s2, 2.0)/pow(s_mag, 4.0) + 
		2*s1/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + pow(s2, 2.0)*s3*(-2*pow(s1, 3.0)/pow(s_mag, 4.0) - 2*s1*pow(s2, 2.0)/pow(s_mag, 4.0) + 
		2*s1/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + 2*pow(s1, 3.0)*s3/(pow(s_mag, 4.0)*s_denom) + 2*s1*pow(s2, 2.0)*s3/(pow(s_mag, 4.0)*s_denom) - 
		2*s1*s3/(pow(s_mag, 2.0)*s_denom) - s3*(pow(s1, 2.0)*(-2*pow(s1, 3.0)/pow(s_mag, 4.0) - 2*s1*pow(s2, 2.0)/pow(s_mag, 4.0) + 
		2*s1/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + pow(s2, 2.0)*(-2*pow(s1, 3.0)/pow(s_mag, 4.0) - 2*s1*pow(s2, 2.0)/pow(s_mag, 4.0) + 
		2*s1/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + 2*pow(s1, 3.0)/(pow(s_mag, 4.0)*s_denom) + 2*s1*pow(s2, 2.0)/(pow(s_mag, 4.0)*s_denom) - 
		2*s1/(pow(s_mag, 2.0)*s_denom)) - s1*s3*(-2*pow(s1, 3.0)/pow(s_mag, 4.0) - 2*s1*pow(s2, 2.0)/pow(s_mag, 4.0) + 
		2*s1/pow(s_mag, 2.0))*x1/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) - 2*pow(s1, 2.0)*s3*x1/(pow(s_mag, 4.0)*s_denom) + s3*x1/(pow(s_mag, 2.0)*s_denom) - 
		s2*s3*(-2*pow(s1, 3.0)/pow(s_mag, 4.0) - 2*s1*pow(s2, 2.0)/pow(s_mag, 4.0) + 2*s1/pow(s_mag, 2.0))*x2/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) - 
		2*s1*s2*s3*x2/(pow(s_mag, 4.0)*s_denom) + (pow(s1, 2.0)*(-2*pow(s1, 3.0)/pow(s_mag, 4.0) - 2*s1*pow(s2, 2.0)/pow(s_mag, 4.0) + 
		2*s1/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + pow(s2, 2.0)*(-2*pow(s1, 3.0)/pow(s_mag, 4.0) - 2*s1*pow(s2, 2.0)/pow(s_mag, 4.0) + 
		2*s1/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + 2*pow(s1, 3.0)/(pow(s_mag, 4.0)*s_denom) + 2*s1*pow(s2, 2.0)/(pow(s_mag, 4.0)*s_denom) - 
		2*s1/(pow(s_mag, 2.0)*s_denom))*x3)/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);

	H_tilde_partial(1, 1) = -(((-pow(s1, 2.0)*s3/(pow(s_mag, 2.0)*s_denom) - pow(s2, 2.0)*s3/(pow(s_mag, 2.0)*s_denom) - s3*(-pow(s1, 2.0)/(pow(s_mag, 2.0)*s_denom) - 
		pow(s2, 2.0)/(pow(s_mag, 2.0)*s_denom)) + s1*s3*x1/(pow(s_mag, 2.0)*s_denom) + s2*s3*x2/(pow(s_mag, 2.0)*s_denom) + (-pow(s1, 2.0)/(pow(s_mag, 2.0)*s_denom) - 
		pow(s2, 2.0)/(pow(s_mag, 2.0)*s_denom))*x3)*(s2/s_mag + s1*s2*(x1-s1)/pow(s_mag, 3.0) + pow(s2, 2.0)*(x2-s2)/pow(s_mag, 3.0) - (x2-s2)/s_mag +  
		s2*s3*(x3-s3)/pow(s_mag, 3.0)))/pow((-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag), 2.0)) + (pow(s1, 2.0)*s3*(-2*pow(s1, 2.0)*s2/pow(s_mag, 4.0) - 
		2*pow(s2, 3.0)/pow(s_mag, 4.0) + 2*s2/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + pow(s2, 2.0)*s3*(-2*pow(s1, 2.0)*s2/pow(s_mag, 4.0) - 
		2*pow(s2, 3.0)/pow(s_mag, 4.0) + 2*s2/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + 2*pow(s1, 2.0)*s2*s3/(pow(s_mag, 4.0)*s_denom) + 
		2*pow(s2, 3.0)*s3/(pow(s_mag, 4.0)*s_denom) - 2*s2*s3/(pow(s_mag, 2.0)*s_denom) - s3*(pow(s1, 2.0)*(-2*pow(s1, 2.0)*s2/pow(s_mag, 4.0) - 
		2*pow(s2, 3.0)/pow(s_mag, 4.0) + 2*s2/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + pow(s2, 2.0)*(-2*pow(s1, 2.0)*s2/pow(s_mag, 4.0) - 
		2*pow(s2, 3.0)/pow(s_mag, 4.0) + 2*s2/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + 2*pow(s1, 2.0)*s2/(pow(s_mag, 4.0)*s_denom) + 
		2*pow(s2, 3.0)/(pow(s_mag, 4.0)*s_denom) - 2*s2/(pow(s_mag, 2.0)*s_denom)) - s1*s3*(-2*pow(s1, 2.0)*s2/pow(s_mag, 4.0) - 2*pow(s2, 3.0)/pow(s_mag, 4.0) + 
		2*s2/pow(s_mag, 2.0))*x1/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) - 2*s1*s2*s3*x1/(pow(s_mag, 4.0)*s_denom) - s2*s3*(-2*pow(s1, 2.0)*s2/pow(s_mag, 4.0) - 
		2*pow(s2, 3.0)/pow(s_mag, 4.0) + 2*s2/pow(s_mag, 2.0))*x2/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) - 2*pow(s2, 2.0)*s3*x2/(pow(s_mag, 4.0)*s_denom) + 
		s3*x2/(pow(s_mag, 2.0)*s_denom) + (pow(s1, 2.0)*(-2*pow(s1, 2.0)*s2/pow(s_mag, 4.0) - 2*pow(s2, 3.0)/pow(s_mag, 4.0) + 
		2*s2/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + pow(s2, 2.0)*(-2*pow(s1, 2.0)*s2/pow(s_mag, 4.0) - 2*pow(s2, 3.0)/pow(s_mag, 4.0) + 
		2*s2/pow(s_mag, 2.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + 2*pow(s1, 2.0)*s2/(pow(s_mag, 4.0)*s_denom) + 2*pow(s2, 3.0)/(pow(s_mag, 4.0)*s_denom) - 
		2*s2/(pow(s_mag, 2.0)*s_denom))*x3)/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);

	H_tilde_partial(1, 2) = -(((-pow(s1, 2.0)*s3/(pow(s_mag, 2.0)*s_denom) - pow(s2, 2.0)*s3/(pow(s_mag, 2.0)*s_denom) - s3*(-pow(s1, 2.0)/(pow(s_mag, 2.0)*s_denom) - 
		pow(s2, 2.0)/(pow(s_mag, 2.0)*s_denom)) + s1*s3*x1/(pow(s_mag, 2.0)*s_denom) + s2*s3*x2/(pow(s_mag, 2.0)*s_denom) + (-pow(s1, 2.0)/(pow(s_mag, 2.0)*s_denom) - 
		pow(s2, 2.0)/(pow(s_mag, 2.0)*s_denom))*x3)*(s3/s_mag + s1*s3*(x1-s1)/pow(s_mag, 3.0) + s2*s3*(x2-s2)/pow(s_mag, 3.0) + pow(s3, 2.0)*(x3-s3)/pow(s_mag, 3.0) - 
		(x3-s3)/s_mag))/pow((-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag), 2.0)) + (pow(s1, 2.0)*s3*(-2*pow(s1, 2.0)*s3/pow(s_mag, 4.0) - 
		2*pow(s2, 2.0)*s3/pow(s_mag, 4.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + pow(s2, 2.0)*s3*(-2*pow(s1, 2.0)*s3/pow(s_mag, 4.0) - 
		2*pow(s2, 2.0)*s3/pow(s_mag, 4.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + 2*pow(s1, 2.0)*pow(s3, 2.0)/(pow(s_mag, 4.0)*s_denom) + 
		2*pow(s2, 2.0)*pow(s3, 2.0)/(pow(s_mag, 4.0)*s_denom) - s3*(pow(s1, 2.0)*(-2*pow(s1, 2.0)*s3/pow(s_mag, 4.0) - 
		2*pow(s2, 2.0)*s3/pow(s_mag, 4.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + pow(s2, 2.0)*(-2*pow(s1, 2.0)*s3/pow(s_mag, 4.0) - 
		2*pow(s2, 2.0)*s3/pow(s_mag, 4.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + 2*pow(s1, 2.0)*s3/(pow(s_mag, 4.0)*s_denom) + 
		2*pow(s2, 2.0)*s3/(pow(s_mag, 4.0)*s_denom)) - s1*s3*(-2*pow(s1, 2.0)*s3/pow(s_mag, 4.0) - 2*pow(s2, 2.0)*s3/pow(s_mag, 4.0))*x1/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) - 
		2*s1*pow(s3, 2.0)*x1/(pow(s_mag, 4.0)*s_denom) + s1*x1/(pow(s_mag, 2.0)*s_denom)- s2*s3*(-2*pow(s1, 2.0)*s3/pow(s_mag, 4.0) - 
		2*pow(s2, 2.0)*s3/pow(s_mag, 4.0))*x2/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) - 2*s2*pow(s3, 2.0)*x2/(pow(s_mag, 4.0)*s_denom) + 
		s2*x2/(pow(s_mag, 2.0)*s_denom) + (pow(s1, 2.0)*(-2*pow(s1, 2.0)*s3/pow(s_mag, 4.0) - 2*pow(s2, 2.0)*s3/pow(s_mag, 4.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + 
		pow(s2, 2.0)*(-2*pow(s1, 2.0)*s3/pow(s_mag, 4.0) - 2*pow(s2, 2.0)*s3/pow(s_mag, 4.0))/(2*pow(s_mag, 2.0)*pow(s_denom, 3.0)) + 
		2*pow(s1, 2.0)*s3/(pow(s_mag, 4.0)*s_denom) + 2*pow(s2, 2.0)*s3/(pow(s_mag, 4.0)*s_denom))*x3)/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);

	H_tilde_partial(2, 0) = -(((pow(s1, 2.0)/s_mag + pow(s2, 2.0)/s_mag + pow(s3, 2.0)/s_mag - s1*x1/s_mag - s2*x2/s_mag - s3*x3/s_mag)*(s1/s_mag + 
		pow(s1, 2.0)*(x1-s1)/pow(s_mag, 3.0) - (x1-s1)/s_mag + s1*s2*(x2-s2)/pow(s_mag, 3.0) + s1*s3*(x3-s3)/pow(s_mag, 3.0)))/pow((-s1*(x1-s1)/s_mag - 
		s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag), 2.0)) + (-pow(s1, 3.0)/pow(s_mag, 3.0) - s1*pow(s2, 2.0)/pow(s_mag, 3.0) - s1*pow(s3, 2.0)/pow(s_mag, 3.0) + 
		2*s1/s_mag + pow(s1, 2.0)*x1/pow(s_mag, 3.0) - x1/s_mag + s1*s2*x2/pow(s_mag, 3.0) + s1*s3*x3/pow(s_mag, 3.0))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);

	H_tilde_partial(2, 1) = -(((pow(s1, 2.0)/s_mag + pow(s2, 2.0)/s_mag + pow(s3, 2.0)/s_mag - s1*x1/s_mag - s2*x2/s_mag - s3*x3/s_mag)*(s2/s_mag + 
		s1*s2*(x1-s1)/pow(s_mag, 3.0) + pow(s2, 2.0)*(x2-s2)/pow(s_mag, 3.0) - (x2-s2)/s_mag + s2*s3*(x3-s3)/pow(s_mag, 3.0)))/pow((-s1*(x1-s1)/s_mag - 
		s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag), 2)) + (-pow(s1, 2.0)*s2/pow(s_mag, 3.0) - pow(s2, 3.0)/pow(s_mag, 3.0) - s2*pow(s3, 2.0)/pow(s_mag, 3.0) + 
		2*s2/s_mag + s1*s2*x1/pow(s_mag, 3.0) + pow(s2, 2.0)*x2/pow(s_mag, 3.0) - x2/s_mag + s2*s3*x3/pow(s_mag, 3.0))/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);

	H_tilde_partial(2, 2) = -(((pow(s1, 2.0)/s_mag + pow(s2, 2.0)/s_mag + pow(s3, 2.0)/s_mag - s1*x1/s_mag - s2*x2/s_mag - s3*x3/s_mag)*(s3/s_mag + 
		s1*s3*(x1-s1)/pow(s_mag, 3.0) + s2*s3*(x2-s2)/pow(s_mag, 3.0) + pow(s3, 2.0)*(x3-s3)/pow(s_mag, 3.0) - (x3-s3)/s_mag))/pow((-s1*(x1-s1)/s_mag - 
		s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag), 2.0)) + (-pow(s1, 2.0)*s3/pow(s_mag, 3.0) - pow(s2, 2.0)*s3/pow(s_mag, 3.0) - pow(s3, 3.0)/pow(s_mag, 3.0) + 
		2*s3/s_mag + s1*s3*x1/pow(s_mag, 3.0) + s2*s3*x2/pow(s_mag, 3.0) + pow(s3, 2.0)*x3/pow(s_mag, 3.0) - x3/s_mag)/(-s1*(x1-s1)/s_mag - s2*(x2-s2)/s_mag - s3*(x3-s3)/s_mag);

	// cout << "H_tilde_partial = \n" << H_tilde_partial << endl;

	mat H_tilde = (DELTA.t() * DELTA).i() * DELTA.t() * T.i() * H_tilde_partial; // dh/ds

	return H_tilde;
}


void SENSOR::plot_ekf()
{
	std::vector<double> x;
	std::vector<double> y;
	std::vector<double> z;

	std::vector<double> sigx;
	std::vector<double> sigy;
	std::vector<double> sigz;
	std::vector<double> nsigx;
	std::vector<double> nsigy;
	std::vector<double> nsigz;

	for(int i = 1; i < ekf.size()-1; i++)
	{
		if(sim == true)
		{
			x.push_back(ekf[i].e_x(0));
			y.push_back(ekf[i].e_x(1));
			z.push_back(ekf[i].e_x(2));

			sigx.push_back(2.0*sqrt(ekf[i].P(0, 0)));
			sigy.push_back(2.0*sqrt(ekf[i].P(1, 1)));
			sigz.push_back(2.0*sqrt(ekf[i].P(2, 2)));
			nsigx.push_back(-2.0*sqrt(ekf[i].P(0, 0)));
			nsigy.push_back(-2.0*sqrt(ekf[i].P(1, 1)));
			nsigz.push_back(-2.0*sqrt(ekf[i].P(2, 2)));
		}
		else
		{
			x.push_back(ekf[i].x_hat(0));
			y.push_back(ekf[i].x_hat(1));
			z.push_back(ekf[i].x_hat(2));

			sigx.push_back(ekf[i].x_hat(0) + 2.0*sqrt(ekf[i].P(0, 0)));
			sigy.push_back(ekf[i].x_hat(1) + 2.0*sqrt(ekf[i].P(1, 1)));
			sigz.push_back(ekf[i].x_hat(2) + 2.0*sqrt(ekf[i].P(2, 2)));
			nsigx.push_back(ekf[i].x_hat(0) - 2.0*sqrt(ekf[i].P(0, 0)));
			nsigy.push_back(ekf[i].x_hat(1) - 2.0*sqrt(ekf[i].P(1, 1)));
			nsigz.push_back(ekf[i].x_hat(2) - 2.0*sqrt(ekf[i].P(2, 2)));
		}


	}

	std::stringstream ttl;

	if(sim == true)
	{
		ttl << "Estimation Error vs No. of Measurement Updates - Sensor " << sid << endl;
	}
	else
	{
		ttl << "Estimation vs No. of Measurement Updates - Sensor " << sid << endl;
	}

	// plt::figure();
	plt::figure_size(1000, 1000);
	plt::suptitle(ttl.str(), {{"fontsize", "20"}});
	plt::subplot(3, 1, 1);
		if(sim == true)
		{
			plt::ylabel("Est. Error - X [m]", {{"fontsize", "16"}});
		}
		else
		{
			plt::ylabel("Estimate - X [m]", {{"fontsize", "16"}});
		}
		plt::grid(true);
		plt::named_plot("Est. Error", x, "b-");
		plt::named_plot("2$\\sigma$", sigx, "r-");
		plt::plot(nsigx, "r-");
		if(sim == true)
		{
			plt::ylim(5*nsigx.back(), 5*sigx.back());
		}
		else
		{
			plt::ylim(x.back() + 5*(nsigx.back() - x.back()), x.back() + 5*(sigx.back() - x.back()));
		}
		plt::legend();
	plt::subplot(3, 1, 2);
		if(sim == true)
		{
			plt::ylabel("Est. Error - Y [m]", {{"fontsize", "16"}});
		}
		else
		{
			plt::ylabel("Estimate - Y [m]", {{"fontsize", "16"}});
		}
		plt::grid(true);
		plt::plot(y, "b-");
		plt::plot(sigy, "r-");
		plt::plot(nsigy, "r-");
		if(sim == true)
		{
			plt::ylim(5*nsigy.back(), 5*sigy.back());
		}
		else
		{
			plt::ylim(y.back() + 5*(nsigy.back() - y.back()), y.back() + 5*(sigy.back() - y.back()));
		}
	plt::subplot(3, 1, 3);
		if(sim == true)
		{
			plt::ylabel("Est. Error - Z [m]", {{"fontsize", "16"}});
		}
		else
		{
			plt::ylabel("Estimate - Z [m]", {{"fontsize", "16"}});
		}
		plt::xlabel("No. of Measurement Updates [k]", {{"fontsize", "16"}});
		plt::grid(true);
		plt::plot(z, "b-");
		plt::plot(sigz, "r-");
		plt::plot(nsigz, "r-");
		if(sim == true)
		{
			plt::ylim(5*nsigz.back(), 5*sigz.back());
		}
		else
		{
			plt::ylim(z.back() + 5*(nsigz.back() - z.back()), z.back() + 5*(sigz.back() - z.back()));
		}
	plt::show();
}


void SENSOR::plot_e_y()
{
	std::vector<double> x;
	std::vector<double> y;
	std::vector<double> z;

	std::vector<double> sigx;
	std::vector<double> sigy;
	std::vector<double> sigz;

	for(int i = 1; i < ekf.size(); i++)
	{
		x.push_back(ekf[i].e_y(0));
		y.push_back(ekf[i].e_y(1));
		z.push_back(ekf[i].e_y(2));
	}

	plt::figure(1);
	plt::plot(x, "b-");
	plt::show();

	plt::figure(2);
	plt::plot(y, "b-");
	plt::show();

	plt::figure(3);
	plt::plot(z, "b-");
	plt::show();
}


// std::vector<std::vector<DATA>> SENSOR::get_sim_data(BAG bag, std::vector<double> t, CORE c)
// {
// 	// Create simulated pixy sensor data of specified markers for a given sensor.
// 	// Simulated data to be used in testing of the position Extended Kalman Filter
// 	//
// 	// Inputs:
// 	//		BAG bag - Object containing the bag information to simulate
// 	//		vector t - [s] Time vector
// 	//		CORE c - Object defining the Core Frame
// 	//
// 	// Outputs:
// 	//		vector<vector<DATA>> Y_sid - Vector containing the measurements from Sensor SID of each marker for each time step k


// 	std::vector<std::vector<DATA>> Y_sid; // Vector of vectors containing Sensor SID measurements
// 	std::vector<DATA> Y_k; // Vector containing Sensor SID measurements at time k
// 	DATA y_mid; // Measurements of Marker MID at time k by Sensor SID

// 	for(int k = 0; k < t.size(); k++) // For each time step
// 	{
// 		Y_k.clear(); // Clear the time measurement vector

// 		for(int j = 0; j < bag.markers.size(); j++) // For each marker
// 		{
// 			std::tuple<double, double> y_k_tmp = get_yhat(bag.markers[j], c, true); // Get the measurement of MID by SID

// 			// Assign measurement fields
// 			y_mid.x = std::get<0>(y_k_tmp);
// 			y_mid.y = std::get<1>(y_k_tmp);
// 			y_mid.mid = j;
// 			y_mid.sid = sid;
			
// 			Y_k.push_back(y_mid); // Add measurement to the vector
// 		}

// 		Y_sid.push_back(Y_k); // Add MID measurements to the vector

// 	}

// 	return Y_sid;
// }