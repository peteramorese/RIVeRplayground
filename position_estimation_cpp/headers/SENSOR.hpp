#ifndef SENSOR_H
#define SENSOR_H

#include "CONSTANTS.hpp"
#include "CORE.hpp"
#include "MARKER.hpp"
#include "BAG.hpp"
#include "DATA.hpp"
#include "EKF.hpp"


#include <armadillo>
using namespace arma;


// Class defining each Pixy2 sensor
class SENSOR
{
public:
	vec position; // [m] Position vector of the sensor
	vec point; // [m] Point that the sensor is pointing at
	mat frame; // Matrix defining the frame of the sensor
	mat Q_C; // DCM to convert from the sensor frame to the Core Frame
	
	const double x_max = 315; // Max x-measurement
	const double y_max = 207; // Max y-measurement
	const double gamma = 30; // [deg] Horizontal FOV half-angle
	const double theta = 20; // [deg] Vertical FOV half-angle
	
	mat R; // Measurement noise matrix
	double dT; // [s] Sampling rate
	int sid; // Sensor ID

	SENSOR();
	SENSOR(vec pos, vec pnt);
	~SENSOR();
	void set_constants();
	mat get_sensor_frame(vec pos, vec pnt, CORE c);
	void calibrate_sensor(std::vector<double> t, std::vector<std::vector<DATA>> Y, BAG b, CORE c, CONSTANTS cnst);
	void set_position(vec pos);
	void init_default_sensors(int id, CORE c);
	std::vector<std::vector<DATA>> get_sim_data(BAG bag, std::vector<double> t, CORE c);
	mat get_H_tilde(MARKER m);
	void plot_ekf(std::vector<double> t);
	void plot_e_y(std::vector<double> t);

private:
	std::vector<EKF> ekf;
	vec position_true; // [m] True position vector of the sensor
	EKF ekf_update(EKF e, BAG bag, std::vector<DATA> y_k, CORE c, CONSTANTS cnst);
	std::tuple<double, double> get_yhat(MARKER m, CORE c, bool sim);
	
	void update_sensor(vec pos_est);
	
};

#endif /* SENSOR_H */