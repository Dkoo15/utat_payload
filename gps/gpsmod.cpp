#include "gpsmod.h"

namespace gps{

	gpsmm* uavgps;
	std::mutex gpsmtx;

	struct position current_loc;
	bool data_is_good;

	struct gps_data_t* data;

	bool startGPS(){
		uavgps = new gpsmm("localhost", DEFAULT_GPSD_PORT);
		if(uavgps->stream(WATCH_ENABLE|WATCH_JSON) == NULL)
			return false;
		else
			return true;
	}

	void readGPS(){

		if(!uavgps->waiting(GPS_WAIT_US)){
			gpsmtx.lock();
			data_is_good = false;
			gpsmtx.unlock();
			return;
		}
			
		data = uavgps->read();
		if(data !=NULL){
			if((data->fix.mode < MODE_2D) || !(data->set)) {
				gpsmtx.lock();
				data_is_good = false;
				gpsmtx.unlock();
			}

			else{
				gpsmtx.lock();
				current_loc.latitude = data->fix.latitude;
				current_loc.longitude = data->fix.longitude;
				current_loc.altitude  = data->fix.altitude;
				current_loc.heading = data->fix.track;
			 	unix_to_iso8601(data->fix.time, current_loc.tbuf, sizeof(current_loc.tbuf));
				data_is_good = false;
				gpsmtx.unlock();
			}
		}
		
	}		
}
