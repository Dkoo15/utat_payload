#include "libgpsmm.h"
#include <iostream>
#include <mutex>

#define GPS_WAIT_US 2000000


struct position{
	double latitude, longitude;
	double altitude, heading;
	char tbuf[200];
};	

class Gps{

	public:	
		Gps();
		~Gps();
		void readGPS();

		struct position current_loc;
		bool data_is_good;
	private: 
		gpsmm* uavgps;
		std::mutex gpsmtx;
		struct gps_data_t* data;
};
