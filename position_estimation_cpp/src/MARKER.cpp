#include "MARKER.hpp"

#include "EKF.hpp"

#include "matplotlibcpp.h"

#include <armadillo>
using namespace arma;
using namespace std;
namespace plt = matplotlibcpp;


MARKER::MARKER()
{
	position = zeros<vec>(3);
	mid = 0;
	plate = 0;

	init_ekf();
}


MARKER::MARKER(vec p, int m, int pl)
{
	position = p;
	mid = m;
	plate = pl;

	init_ekf();
}


MARKER::~MARKER(){};


void MARKER::init_ekf()
{
	EKF e;
	e.x_hat = position;
	e.P = {{pow(5, 2), 0, 0},
			{0, pow(1.195, 2), 0},
			{0, 0, pow(1.195, 2)}};

	ekf.push_back(e);
}


void MARKER::estimate_pos(vector<vector<DATA>> Y, vector<SENSOR_MIN> sensors, CORE core, CONSTANTS cnst)
{

	EKF e;
	vector<DATA> y_k;

	for(int k = 0; k < Y.size(); k++)
	{
		// cout << "Y " << Y.size() << endl;
		y_k = Y[k];
		// cout << "sensors " << sensors[0].position << endl;
		e = ekf_update(ekf[ekf.size()-1], sensors, y_k, core, cnst);
		// cout << "After" << endl;
	}

}


EKF MARKER::ekf_update(EKF e, vector<SENSOR_MIN> sensors, vector<DATA> y_k, CORE core, CONSTANTS cnst)
{
	EKF e_kp1;

	return e_kp1;
}


void MARKER::plot_ekf()
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
		ttl << "Estimation Error vs No. of Measurement Updates - Marker " << mid << endl;
	}
	else
	{
		ttl << "Estimation vs No. of Measurement Updates - Marker " << mid << endl;
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


void MARKER::plot_e_y()
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
