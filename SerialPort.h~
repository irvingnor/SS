#ifndef SERIALPORT_H
#define SERIALPORT_H
#define ERROR -1

#include <termios.h>	//UNIX API for terminal I/O
#include <fcntl.h>	// Constant declarations for termios functions
#include <unistd.h> 	//close() function
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>	//needed for memset
#include <thread>

using namespace std;

void PrintError(int errorCode);
int OpenPort(char* portName);
int ConfigurePort(int portDescriptor);
int WriteToSerial(int portDescriptor,char* buffer,int numberOfBytes);
int ReadFromSerial(int portDescriptor,char* buffer,int numberOfBytes);
void StartThreadedRead(int portDescriptor,char* buffer,int numberOfBytes);
void ThreadedRead(int portDescriptor,char* buffer,int numberOfBytes);

enum Errors
{
	ERROR_OPEN,
	ERROR_SET_SPEED,
	ERROR_CONFIGURE_PORT,
	ERROR_WRITE,
	ERROR_READ

};


#endif
