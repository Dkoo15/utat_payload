#include <iostream>
#include <mutex>

#include "libgpsmm.h"
#include "gps_struct.h"

#define GPS_WAIT_US 2000000

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
