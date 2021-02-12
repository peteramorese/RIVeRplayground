#ifndef MARKER_H
#define MARKER_H

#include "EKF.hpp"
#include "DATA.hpp"
#include "SENSOR_MIN.hpp"
#include "CORE.hpp"
#include "CONSTANTS.hpp"

#include <armadillo>
using namespace arma;

class SENSOR;

// Struct defining each bag marker
class MARKER
{
public:
	vec position;
	int mid;
	bool plate;
	bool sim;
	bool updated = false;

	MARKER();
	MARKER(vec p, int m, int pl);
	~MARKER();
	void estimate_pos(vector<vector<DATA>>, vector<SENSOR_MIN>, CORE, CONSTANTS);

private:
	std::vector<EKF> ekf;

	void init_ekf();
	EKF ekf_update(EKF, vector<SENSOR_MIN>, vector<DATA>, CORE, CONSTANTS);
	tuple<double, double> get_yhat(SENSOR_MIN, CORE, bool);
	mat get_H_tilde(SENSOR_MIN);
	void plot_ekf();
	void plot_e_y();

	
};

#endif /* MARKER_H */