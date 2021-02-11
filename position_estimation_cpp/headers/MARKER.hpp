#ifndef MARKER_H
#define MARKER_H

#include "EKF.hpp"

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

	
};

#endif /* MARKER_H */