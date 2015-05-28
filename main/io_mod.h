#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#define CONFIG_FILE "uav/utat_payload/options.cfg"

extern int cameratype, usegps, saveimg, view, sizefac, jpgq, bufferq, timeout, start_delay, delay, imgstrm;
extern double location[4];

bool parseConfig();
int checkLogInit();
void writeLine(std::ofstream &logstream, std::string image);
