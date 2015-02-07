#include "gpsmod.h"

#include <iostream>
using namespace std;
namespace gps{

	gpsmm* uavgps;
	struct gps_data_t* data;
	bool startGPS(){
		uavgps = new gpsmm("localhost", DEFAULT_GPSD_PORT);
		if(uavgps->stream(WATCH_ENABLE|WATCH_JSON) == NULL)
			return false;
		else
			return true;
	
	}

	bool getGPS(double (&loc)[4]){
		if(!uavgps->waiting(GPS_WAIT_US)) return false;
		
		data = uavgps->read();	

		loc[0] = 0;
		loc[1] = 0;
		loc[2] = 0;
		loc[3] = 0;

		if(data  == NULL) 
			return false;

		else if((data->fix.mode < MODE_2D) || !(data->set))
			return false;
		else{
			loc[0] = data->fix.latitude;
			loc[1] = data->fix.longitude;
			loc[2] = data->fix.track;
			loc[3] = data->fix.altitude;
			return true;
		}
		return true;
	}		
}
