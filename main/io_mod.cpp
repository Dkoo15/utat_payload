#include "io_mod.h"

bool parseConfig(){
	std::ifstream cfgstream(CONFIG_FILE, std::ifstream::in);
	std::string param, num;
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
			sizefac = std::stoi(num);
			continue;
		}
		if(param.compare("jpgq")==0){
			std::getline(cfgstream,num);
			jpgq  = std::stoi(num);
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
		if(param.compare("imgstrm")==0){
			std::getline(cfgstream,num);
			imgstrm = std::stoi(num);
			continue;
		}
		if(param.compare("gain")==0){
			std::getline(cfgstream,num,',');
			rawgain[0] = std::stof(num);
			std::getline(cfgstream,num,',');
			rawgain[1] = std::stof(num);
			std::getline(cfgstream,num,',');
			rawgain[2] = std::stof(num);
			std::getline(cfgstream,num);
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


