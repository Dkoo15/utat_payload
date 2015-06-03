#include "libgpsmm.h"
#include <mutex>

#define GPS_WAIT_US 2000000


struct position{
	double latitude, longitude;
	double altitude, heading;
	char tbuf[200];;
};	

namespace gps{
	
	extern struct position current_loc;
	extern bool data_is_good;

	bool startGPS();
	void readGPS();

}
