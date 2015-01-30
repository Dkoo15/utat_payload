#include "gpsmod.h"

namespace gps{

	gpsmm* uavgps;

	bool startGPS(){
		uavgps = new gpsmm("localhost", DEFAULT_GPSD_PORT);
		
		if(uavgps->stream(WATCH_ENABLE|WATCH_JSON) == NULL)
			return false;
		else
			return true;
	}

	bool getGPS(double (&loc)[4]){
		struct gps_data_t* newdata;
		
		std::cout<<"Polling GPS..."<<std::endl;
		
		if(!uavgps->waiting(GPS_WAIT_US)) return false;
			
		if((newdata = uavgps->read()) == NULL) return false;

		else{
			loc[0] = newdata->fix.latitude;
			loc[1] = newdata->fix.longitude;
			loc[2] = newdata->fix.track;
			loc[3] = newdata->fix.altitude;
			return true;
		}
	}		
}
