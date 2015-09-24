#include "SerialPort.h"


int main()
{
	char* name="/dev/ttyUSB0";
	char buffer1[1]= {(char)128};
	char buffer2[1]= {(char)0x83};
	char buffer3[5]= {(char)0x91,(char)0x00,(char)0x1F,(char)0x00,(char)0x1F};
	char buffer4[2]= {(char)0x88, (char)0x08};
	int desc=OpenPort(name);
	ConfigurePort(desc);

	WriteToSerial(desc,buffer1,sizeof(buffer1));
	WriteToSerial(desc,buffer2,sizeof(buffer2));
	usleep(120000);
	WriteToSerial(desc,buffer3,sizeof(buffer3));
	sleep(8);
	WriteToSerial(desc,buffer4,sizeof(buffer4));


	/*char readBuffer[100];
	StartThreadedRead(desc,readBuffer,sizeof(readBuffer));
	*/
return 0;
}
