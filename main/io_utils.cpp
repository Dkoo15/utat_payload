#include "io_utils.h"

std::ofstream gps_log;

bool parseConfig(){
	std::ifstream cfgstream(CONFIG_FILE, std::ifstream::in);
	std::string param, num;
	int tmp;
	bool cfg_ok = false;
	if(!cfgstream) return cfg_ok;

	while(std::getline(cfgstream,param,'=')) {
		if (param.compare("cameratype")==0){
			std::getline(cfgstream,num);
			cameratype = std::stoi(num);
			continue;
		}
		if (param.compare("usegps")==0){
			std::getline(cfgstream,num);
			usegps = std::stoi(num);
			continue;
		}
		if(param.compare("savepicture")==0){
			std::getline(cfgstream,num);
			saveimg = std::stoi(num);
			continue;
		}
		if(param.compare("sizefactor")==0){
			std::getline(cfgstream,num);
			tmp = std::stoi(num);
			sizefac = double(1/double(tmp));
			continue;
		}
		if(param.compare("viewer")==0){
			std::getline(cfgstream,num);
			view = std::stoi(num);
			continue;
		}
		if(param.compare("start_delay")==0){
			std::getline(cfgstream,num);
			start_delay = std::stoi(num);
			continue;
		}
		if(param.compare("stream")==0){
			std::getline(cfgstream,num);
			strm = std::stoi(num);
			continue;
		}
	}
	cfg_ok = true;

	cfgstream.close();
	return cfg_ok;
}

int checkLog(){
	int saved = 0;
	std::ifstream logstream (LOG_FILE, std::ifstream::in);
	
	if(!logstream){
		std::cout << "Starting from scratch" << std::endl;
		return -1;
		}
	else {
		std::string line;
		if (std::getline(logstream,line)){
			while(std::getline(logstream,line))  saved++;
		}
		logstream.close();
		std::cout << "Starting after " << saved << " images" << std::endl;
	    }
	return saved;
}

void openLogtoWrite(int n_saved){
	gps_log.open(LOG_FILE,std::ofstream::app);
	gps_log<< std::fixed;

	if (n_saved == -1)
		gps_log <<"Image,Latitude[deg],Longitude[deg],Altitude[m],Heading[deg],Time" << std::endl;
	
}

void writeImageInfo(struct position latest_gps, std::string image){
	gps_log<< image  <<",";
	gps_log.precision(9);
	gps_log<< latest_gps.latitude <<","<< latest_gps.longitude<<",";
	gps_log.precision(2);
	gps_log<< latest_gps.altitude <<","<< latest_gps.heading << ",";
	gps_log<< latest_gps.time<<std::endl;
}

void closeLog(){
	gps_log.close();
}
