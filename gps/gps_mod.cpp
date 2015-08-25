#include "gps_mod.h"

Gps::Gps(){
	uavgps = new gpsmm("localhost", DEFAULT_GPSD_PORT);

	current_loc.latitude = 0;
	current_loc.longitude = 0;
	current_loc.altitude  = 0;
	current_loc.heading = 0;
	current_loc.time = new char[100];

	if(uavgps->stream(WATCH_ENABLE|WATCH_JSON) == NULL)
		std::cout<<"Error opening GPS" << std::endl; 
	else
		std::cout<<"GPS is ok" << std::endl;	
}

Gps::~Gps(){
	delete current_loc.time;
}

void Gps::readGPS(){

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
			unix_to_iso8601(data->fix.time, current_loc.time, sizeof(current_loc.time));
			data_is_good = true;
			gpsmtx.unlock();
		}
	}
	
}		
