#include "libgpsmm.h"
#include <iostream>

#define GPS_WAIT_US 750000

namespace gps{

	bool startGPS();
	bool getGPS(double (&loc)[4]);

}
