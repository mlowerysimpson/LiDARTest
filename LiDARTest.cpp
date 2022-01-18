//#include "cv.h"
//include "cvaux.hpp"
//#include "TFMini.h" //--> uncomment if using TF Mini
#include "../RemoteControlTest/LIDARLite.h"
#include "../RemoteControlTest/Util.h"
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <cstring>
#include <math.h>

#include <iostream>


using namespace std;

void ShowUsage(int nNumMeasurements) {
	printf("LiDARTest\n");
	printf("Usage: LiDARTest [-p 0 or 1]\n");
	printf("Running the LiDARTest program without any arguments powers up the LiDAR device and collects %d measurements of distance data.\n",nNumMeasurements);
	printf ("The optional 'p' command flag can be used to turn power to the LiDAR off when it is followed by a zero, or on when followed by any other number.\n");
}

int main(int argc, const char* argv[])
{
	const int NUM_MEASUREMENTS = 1000;
	if (wiringPiSetup () == -1) {
		printf ("Unable to start wiringPi.\n");
		return -1 ;
	}

	bool bContainsHelpFlag = Util::ContainsHelpFlag(argc, argv);
	if (bContainsHelpFlag) {
		ShowUsage(NUM_MEASUREMENTS);
		return 1;
	}

	//uncomment the following block if using TFMini
	/*
  	int fd = serialOpen ("/dev/serial0", 115200);
	if (fd<0) {
		printf ("Unable to open serial device: %s\n", strerror (errno));
		return -2;
	}*/
	
	//TFMini lidarDevice; //--> uncomment if using TF Mini
	LIDARLite lidarDevice(nullptr);
	printf("Initializing...\n");
	//Initialize the LiDAR
	//if (!lidarDevice.begin(fd)) { //--> uncomment if using TF Mini
	if (lidarDevice.m_bInitError) {
		printf("error trying to initialize the LiDAR.\n");
		return -3;
	}
	bool bSetPowerFlag = false;
	int nPowerCommand = 0;
	if (argc>=3) {
		if  (strstr(argv[1],(char *)"-p")) {
			bSetPowerFlag = true;
			sscanf(argv[2],"%d",&nPowerCommand);
		}
		else if (strstr(argv[1],(char *)"-P")) {
			bSetPowerFlag = true;
			sscanf(argv[2],"%d",&nPowerCommand);
		}
		if (bSetPowerFlag) {
			if (nPowerCommand>0) {
				lidarDevice.TurnOn(true);//turn on LiDAR device
				printf("LiDAR device turned on.\n");
			}
			else {
				lidarDevice.TurnOn(false);//turn off LiDAR device
				printf("LiDAR device turned off.\n");
			}
			return 0;
		}
	}
	for (int i=0;i<NUM_MEASUREMENTS;i++) {
		uint16_t dist = (uint16_t)lidarDevice.getDistance();
		//uint16_t strength = lidarDevice.getRecentSignalStrength(); //can uncomment to get signal strength if using TFMini
		printf ("dist = %d cm\n",dist);
		delay(100);//pause for 100 ms between measurements
	}
	return 0;
}
