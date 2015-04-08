#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

#define CONFIG_FILE "uav/utat_payload/options.cfg"

extern int width, height;
extern int usecamera, usegps, saveimg, view;
extern int sizefac, jpgq;
extern int bufferq, timeout;
extern int start_delay, delay;
extern double location[4];
extern int ip;

bool parseConfig();
int checkLogInit();
void writeLine(std::ofstream &logstream);
