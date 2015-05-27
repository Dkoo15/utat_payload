#include "libgpsmm.h"

#define GPS_WAIT_US 2000000

namespace gps{

	bool startGPS();
	void getGPS(double (&loc)[4]);

}
