#include "io_mod.h"

bool parseConfig(){
	std::ifstream cfgstream(CONFIG_FILE, std::ifstream::in);
	std::string param, num;
	bool cfg_ok = false;
	int val;
	if(!cfgstream) return cfg_ok;

	while(std::getline(cfgstream,param,'=')) {
		std::getline(cfgstream,num);
		val = std::stoi(num);
		if (param.compare("cameratype")==0){
			cameratype = val;
			continue;
		}
		if (param.compare("usegps")==0){
			usegps = val;
			continue;
		}
		if(param.compare("savepicture")==0){
			saveimg = val;
			continue;
		}
		if(param.compare("sizefactor")==0){
			sizefac = val;
			continue;
		}
		if(param.compare("delay")==0){
			delay = val;
			continue;
		}
		if(param.compare("jpgq")==0){
			jpgq = val;		
			continue;
		}
		if(param.compare("viewer")==0){
			view = val;
			continue;
		}
		if(param.compare("start_delay")==0){
			start_delay = val;
			continue;
		}
		if(param.compare("imgstrm")==0){
			imgstrm = val;
			continue;
		}

	}
	cfg_ok = true;

	cfgstream.close();
	return cfg_ok;
}

int checkLogInit(){
	int saved = 0;
	std::ifstream logstream ("Pictures/uav_gps.log", std::ifstream::in);
	
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
