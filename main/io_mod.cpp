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
		if (param.compare("width") ==0){
			width = val;
			continue;
		}		
		if (param.compare("height") ==0){
			height = val;
			continue;
		}		
		if (param.compare("usecamera")==0){
			usecamera = val;
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
		if(param.compare("bufferq")==0){
			bufferq = val;
			continue;
		}
		if(param.compare("timeout")==0){
			timeout = val;
			continue;
		}
		if(param.compare("start_delay")==0){
			start_delay = val;
			continue;
		}
	}
	cfg_ok = true;

	cfgstream.close();
	return cfg_ok;
}

int checkLogInit(){
	int saved = 0;
	std::ifstream logstream ("uav/save/uav_gps.log", std::ifstream::in);
	
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

void writeLine(std::ofstream &logstream){
	logstream<<"im";
	logstream<<std::setfill('0')<<std::setw(4)<<ip;
	logstream<< ".jpg" <<"\t";
	logstream<<std::setprecision(8);
	logstream<< location[0]<<"\t";
	logstream<<std::setprecision(8);
	logstream<< location[1]<<"\t";
	logstream<< location[2]<<"\t\t";
	logstream<< location[3]<<"\t";
	logstream << std::endl;
}

