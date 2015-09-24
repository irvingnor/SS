#ifndef ROBOT_H
#define ROBOT_H

#include"SerialPort.h"
#include"Nota.h"
#include<string>
#include<sstream>
#include<vector>
#include<stdio.h>
#include<thread>

using namespace std;

class Robot
{
	public:
		Robot(string);
		~Robot();
		string getNombrePuerto();
		bool estaCargando();
		void start();
		void baud(unsigned char);
		void control();
		void safe();
		void full();
		void spot();
		void cover();
		void demo(unsigned char);
		void drive(int,int);
		void lowSideDrivers(unsigned char);
		void leds(unsigned char,unsigned char,unsigned char);
		void song(unsigned char,vector<Nota>);
		void playSong(unsigned char);
		void sensors(unsigned char);
		int getSizePacket(int);
		void coverAndDock();
		void pwmLowSideDrivers(unsigned char,unsigned char,unsigned char);
		void driveDirect(int,int);
		void digitalOutputs(unsigned char);
		void stream(vector<unsigned char>);
		//void ThreadedReadStream(int,char*,int);
		void queryList(vector<unsigned char>);
		void pauseResumeStream(bool);
		void sendIr(unsigned char);
		void script(vector<unsigned char>);
		void playScript();
		void showScript();
		void waitTime(unsigned char);
		void waitDistance(int);
		void waitAngle(int);
		void waitEvent(signed char);		
		void error(int,void*);
		int getBaudCode(int);
		char* charModo(int);
		bool getBumpRight();
		bool getBumpLeft();
		bool getWheelDropRight();
		bool getWheelDropLeft();
		bool getWheelDropCaster();
		bool getWallSeen();
		bool getCliffLeft();
		bool getCliffFrontLeft();
		bool getCliffFrontRight();
		bool getCliffRight();
		bool getVirtualWall();
		int getDistance();
		bool getLd0();
		bool getLd1();
		bool getLd2();
		bool getRightWheel();
		bool getLeftWheel();
		bool getEstadoStream();
	private:
		void readBumpsAndWheelDrops(char*);
		void readWall(char*);
		void readCliffLeft(char*);
		void readCliffFrontLeft(char*);
		void readCliffFrontRight(char*);
		void readCliffRight(char*);
		void readVirtualWall(char*);
		void readLSDriverAndWheelO(char*);
		void readDistance(char*);
		void actualizaSensor(unsigned char, int);
		string nombrePuerto;
		int modo;
		bool cargando;
		int descriptorPuerto;
		int baudRate;
		bool bumpRight;
		bool bumpLeft;
		bool wheelDropRight;
		bool wheelDropLeft;
		bool wheelDropCaster;
		bool wall;
		bool cliffLeft;
		bool cliffFrontLeft;
		bool cliffFrontRight;
		bool cliffRight;
		bool virtualWall;
		int distance;
		bool leftWheel;//left and right Wheel son parte de readLSDriverAndWheelO
		bool rightWheel;
		bool ld2;
		bool ld1;
		bool ld0;
		bool estadoStream;
};

#endif
