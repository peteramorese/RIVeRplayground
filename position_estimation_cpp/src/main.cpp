#include "SYSTEM.hpp"
using namespace std;

int main()
{

// Default settings
bool sim = true; // Set run mode to simulation
bool plot = false; // Set the plot flag
	

bool menu = true;
int opt;

while(menu == true)
{
	cout << "\n\n\t\tInitialization Menu\t\t" << endl;
	cout << "----------------------------------------------------------" << endl;
	cout << "1) Settings" << endl;
	cout << "2) Initialize System\n" << endl;

	cin >> opt;

	cout << endl;

	switch(opt)
	{
		case 1 :
		{
			bool submenu = true;

			while(submenu == true)
			{
				int set;
				cout << "Select a Setting to Change:" << endl;
				cout << "1) Software Run Mode: ";
				if(sim == true)
				{
					cout << "Simulation" << endl;
				}
				else if(sim == false)
				{
					cout << "Flight" << endl;
				}
				cout << "2) Plotting: " << plot << endl;
				cout << "0) Back to the Menu\n" << endl;

				cin >> set;

				cout << endl;

				switch(set)
				{
					case 1 :
					{
						int mode;

						cout << "Software Run Mode:" << endl;
						cout << "1) Simulation" << endl;
						cout << "2) Flight\n" << endl;

						cin >> mode;

						cout << endl;

						if(mode == 1)
						{
							sim = true;
						}
						else
						{
							sim = false;
						}

						break;
					}
					case 2 :
					{
						int plot_flag;

						cout << "Plotting:" << endl;
						cout << "1) Display Plots" << endl;
						cout << "2) Suppress Plots\n" << endl;

						cin >> plot_flag;

						cout << endl;

						if(plot_flag == 1)
						{
							plot = true;
						}
						else
						{
							plot = false;
						}

						break;
					}
					case 0 :
					{
						submenu = false;
						break;
					}
				}
			}
			break;
		}
		cout << "opt = " << opt << endl;
		case 2 :
		{
			menu = false;
			break;
		}
		default :
		{
			cout << "Invalid Selection" << endl;
			break;
		}
	}
	cout << "\n\n\n\n\n\n" << endl;
}

// Initialize the System
SYSTEM system(sim, plot);

// Main Menu
menu = true;

while(menu == true)
{
	cout << "\n\n\t\tMENU\t\t" << endl;
	cout << "-------------------------------------------" << endl;
	cout << "1) Calibrate Pickup Sensors" << endl;
	cout << "2) Calibrate Dropoff Sensors" << endl;
	cout << "3) Scan Pickup" << endl;
	cout << "4) Scan Dropoff" << endl;
	cout << "0) Exit\n" << endl;

	cin >> opt;

	cout << endl;

	switch(opt)
	{
		case 1 :
		{
			// Initialize a bag to calibrate the sensors containing 3 markers
			BAG calibrator_p(3);
			calibrator_p.markers[0].position = {0, 0.1, 0.1};
			calibrator_p.markers[1].position = {0.05, -0.05, 0.1};
			calibrator_p.markers[2].position = {0.1, -0.05, 0};

			// Calibrate sensors 0 - 4
			system.calibrate_pickup(calibrator_p);

			break;
		}
		case 2 :
		{
			// Initialize a bag to calibrate the dropoff sensors
			BAG calibrator_d(6);
			calibrator_d.markers[3].position = {2.25, 0.85, -0.80};
			calibrator_d.markers[4].position = {2.50, 0.90, -0.8};
			calibrator_d.markers[5].position = {2.30, 0.75, -0.75};

			// Calibrate sensors 5 - 9
			system.calibrate_dropoff(calibrator_d);	

			break;
		}
		case 3:
		{
			BAG bag1(6);
			system.assign_bag(bag1);

			system.run_estimator_pickup();

			break;
		}
		case 4 :
		{
			BAG bag2(6);
			system.assign_bag(bag2);

			system.run_estimator_dropoff();
			
			break;
		}
		case 0 :
		{
			cout << "Exiting..." << endl;
			menu = false;
			break;
		}
		default :
		{
			cout << "Invalid Selection" << endl;
		}
	}

}




// Calibrate the Sensor System
	// system.calibrate(calibrator);

// Initialize the new cargo bag
	// BAG bag1(6);
	// system.assign_bag(bag1);

// Run the bag estimator
	// system.run_estimator();

}


