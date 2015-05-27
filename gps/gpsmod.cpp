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

	void getGPS(double (&loc)[4]){
		loc[0] = 0.0;
		loc[1] = 0.0;
		loc[2] = 0.0;
	 	loc[3] = 0.0;	

		if(!uavgps->waiting(GPS_WAIT_US)){
			std::cout<<"No GPS Data available"<<std::endl;
			return;
		}
			
			

		do{	
			data = uavgps->read();
			std::cout<<"gps buffer: ";
			if(data !=NULL){
				if((data->fix.mode < MODE_2D) || !(data->set)) 
					std::cout<<"Poor GPS fix" <<std::endl;
				else{
					loc[0] = data->fix.latitude;
					loc[1] = data->fix.longitude;
					loc[2] = data->fix.altitude;
					loc[3] = data->fix.track;
					std::cout<<"Latitude: " << loc[0] << "  Longitude: " <<loc[1]<<std::endl;
				}
			}
		}while(data!=NULL);

		if(data  == NULL) 
			std::cout<<"Null Data"<<std::endl;
	}		
}
