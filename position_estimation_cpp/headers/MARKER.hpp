#ifndef MARKER_H
#define MARKER_H

#include "EKF.hpp"
#include "DATA.hpp"
#include "CORE.hpp"
#include "CONSTANTS.hpp"

#include <armadillo>
using namespace arma;


// Struct defining each bag marker
class MARKER
{
public:
	vec position;
	int mid;
	bool plate;

	MARKER();
	MARKER(vec p, int m, int pl);
	~MARKER();

private:
	std::vector<EKF> ekf;

	void init_ekf();
	void estimate_pos(vector<vector<vector<DATA>>>, CORE, CONSTANTS);

	
};

#endif /* MARKER_H */