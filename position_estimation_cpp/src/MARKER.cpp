#include "MARKER.hpp"

#include <armadillo>
using namespace arma;
using namespace std;

MARKER::MARKER()
{
	position = zeros<vec>(3);
	mid = 0;
	plate = 0;
}

MARKER::MARKER(vec p, int m, int pl)
{
	position = p;
	mid = m;
	plate = pl;
}

MARKER::~MARKER(){};