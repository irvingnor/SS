#include"Robot.h"

enum errores{DEMO_INVALIDO,BAUD_INVALIDO,MODO_INVALIDO_INSTRUCCION,FUERA_RANGO,MODO_INVALIDO};
enum modos{OFF,PASSIVE,SAFE,FULL};
enum baudCode{BAUD300,BAUD600,BAUD1200,BAUD2400,BAUD4800,BAUD9600,BAUD14400,BAUD19200,BAUD28800,BAUD38400,BAUD57600,BAUD115200};

//--------
void ThreadedReadStream(Robot *r,int portDescriptor,char*buffer,int numberOfBytes)
{
	while(r->getEstadoStream())
	{
		usleep(120000);
		ReadFromSerial(portDescriptor,buffer,numberOfBytes);
		usleep(120000);
		string cadena(buffer);
		cout<<(unsigned int)buffer[0]<<endl;
		cout<<(unsigned int)buffer[1]<<endl;
		cout<<(unsigned int)buffer[2]<<endl;
		cout<<(unsigned int)buffer[3]<<endl;
		cout<<(unsigned int)buffer[4]<<endl;
		cout<<(unsigned int)buffer[5]<<endl;
		cout<<(unsigned int)buffer[6]<<endl;
		cout<<(unsigned int)buffer[7]<<endl;
		//cout<<cadena<<endl;
		cout<<"----------"<<endl;
	}
}
//-----------
Robot::Robot(string nombrePuerto1)
{
	nombrePuerto = nombrePuerto1;
	descriptorPuerto = OpenPort(const_cast<char *>(nombrePuerto.c_str()));
	ConfigurePort(descriptorPuerto);
	estadoStream = false;
	start();
}

Robot::~Robot()
{
	
}

string Robot::getNombrePuerto()
{
	return nombrePuerto;
}

bool Robot::estaCargando()
{
	return cargando;
}

void Robot::start()
{
	char ins = (char)0x80;
	WriteToSerial(descriptorPuerto,&ins,1);
	modo = PASSIVE;
}

void Robot::baud(unsigned char baudCode)
{
	if(baudCode>=0 && baudCode<=11)
	{
		char ins[2]{(char)0x81,(char)baudCode};
		WriteToSerial(descriptorPuerto,ins,2);
		usleep(100000);
		baudRate = getBaudCode(baudCode);
	}
	else
		error(BAUD_INVALIDO,&baudCode);
}

void Robot::control()
{
	safe();
}

void Robot::safe()
{
	char ins = (char)0x83;
	WriteToSerial(descriptorPuerto,&ins,1);
	modo = SAFE;
}

void Robot::full()
{
	char ins = (char)0x84;
	WriteToSerial(descriptorPuerto,&ins,1);
	modo = FULL;
}

void Robot::spot()
{
	char ins = (char)0x86;
	WriteToSerial(descriptorPuerto,&ins,1);
	modo = PASSIVE;
}

void Robot::cover()
{
	char ins = (char)0x87;
	WriteToSerial(descriptorPuerto,&ins,1);
	modo = PASSIVE;
}

void Robot::demo(unsigned char demo)
{
	if(demo>0 && demo<=9)
	{
		char ins[2] = {(char)0x88,(char)demo};
		WriteToSerial(descriptorPuerto,ins,2);
	}
	else
		error(DEMO_INVALIDO,&demo);
}

void Robot::drive(int velocity,int radius)
{
	if(modo == SAFE || modo == FULL)
	{
		char arreglo[2];
		memcpy(arreglo,&velocity,2);
		char arreglo2[2];
		memcpy(arreglo2,&radius,2);
		char ins[5] = {(char)0x89,(char)arreglo[1],(char)arreglo[0],(char)arreglo2[1],(char)arreglo2[0]};
		WriteToSerial(descriptorPuerto,ins,5);		
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion DRIVE");
}

void Robot::lowSideDrivers(unsigned char driverBits)
{
	//Probar funcion 1/7
	if(modo == SAFE || modo == FULL)
	{
		if(driverBits>=0 && driverBits<=2)
		{
			char ins[2] = {(char)0x8A,(char)driverBits};
			WriteToSerial(descriptorPuerto,ins,2);
		}
		else
			error(FUERA_RANGO,(char*)"Funcion LOWSIDEDRIVERS");
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion LOWSIDEDRIVERS");
}

void Robot::leds(unsigned char bit,unsigned char color,unsigned char intensity)
{
	if(modo == SAFE || modo == FULL)
	{
		if(bit == 2 || bit == 8 || bit == 10 || bit == 0)
		{
			char ins[4]{(char)0x8B,(char)bit,(char)color,(char)intensity};
			WriteToSerial(descriptorPuerto,ins,4);
		}
		else
		{
			error(FUERA_RANGO,(char*)"Funcion LEDS");
		}
	}
	else
	{ 
		string cadena(charModo(modo));	
		cadena += " en la funcion LEDS"; 
		error(MODO_INVALIDO_INSTRUCCION,const_cast<char*>(cadena.c_str()));	
	}
}

void Robot::song(unsigned char songNumber,vector<Nota> notas)
{
	unsigned char* ins = new unsigned char[notas.size()*2+3];
	ins[0] = (unsigned char)0x8C;
	ins[1] = (unsigned char)songNumber;
	ins[2] = (unsigned char)notas.size();
	int size = notas.size();
	if(songNumber>=0 && songNumber<=15 && notas.size()>=1 && notas.size()<=16)
	{
		for(int i=3,j=0;j<size;i+=2,j++)
		{
			ins[i] = notas[j].getNota();
			ins[i+1] = notas[j].getDuracion();
		}
		WriteToSerial(descriptorPuerto,(char*)ins,notas.size()*2+3);
	}
	else
		error(FUERA_RANGO,(char*)"Funcion Song");
}

void Robot::playSong(unsigned char songNumber)
{
	if(modo == SAFE || modo == FULL)
	{
		if(songNumber>=0 && songNumber<=15)
		{
			char ins[2]{(char)0x8D,(char)songNumber};
			WriteToSerial(descriptorPuerto,ins,2);
		}
		else
		{
			error(FUERA_RANGO,(char*)"Funcion PLAYSONG");
		}
	}
	else
	{ 
		string cadena(charModo(modo));	
		cadena += " en la funcion PLAYSONG"; 
		error(MODO_INVALIDO_INSTRUCCION,const_cast<char*>(cadena.c_str()));	
	}
}

void Robot::sensors(unsigned char idPacket)
{
	if(idPacket>=0 && idPacket<=42)
	{
		char ins[2]{(char)0x8E,(char)idPacket};
		WriteToSerial(descriptorPuerto,ins,2);
		int sizePacket = getSizePacket(idPacket);
		actualizaSensor(idPacket,sizePacket);
	}
	else
		error(FUERA_RANGO,(char*)"Funcion SENSORS");
}

int Robot::getSizePacket(int idPacket)
{
	int sizePacket = 0;
	switch(idPacket)
	{
		case 7:case 8:case 9:case 10:case 11:case 12:case 13:
		case 14:case 15:case 16:case 17:case 18:case 21:case 24:
		case 32:case 34:case 35:case 36:case 37:case 38:
			sizePacket = 1;
		break;
		case 19:case 20:case 22:case 23:case 25:case 26:case 27:case 28:
		case 29:case 30:case 31:case 33:case 39:case 40:case 41:case 42:
			sizePacket = 2;
		break;
	}
	return sizePacket;
}

void Robot::coverAndDock()
{
	char ins = (char)0x8F;
	WriteToSerial(descriptorPuerto,&ins,1);
	modo = PASSIVE;
}

void Robot::pwmLowSideDrivers(unsigned char lowSideDriver2,unsigned char lowSideDriver1,unsigned char lowSideDriver0)
{
	//Probar funcion 2/7
	if(modo == SAFE || modo == FULL)
	{
		if(lowSideDriver2<=128 && lowSideDriver2>=0 &&
		lowSideDriver1<=128 && lowSideDriver1>=0 &&
		lowSideDriver0<=128 && lowSideDriver0>=0)
		{
			char ins[4] = {(char)0x90,(char)lowSideDriver2,(char)lowSideDriver1,(char)lowSideDriver0};
			WriteToSerial(descriptorPuerto,ins,4);
		}
		else
			error(FUERA_RANGO,(char*)"Funcion PWMLOWSIDEDRIVERS");
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion PWMLOWSIDEDRIVERS");
}

void Robot::driveDirect(int rightVelocity,int leftVelocity)
{
	if(modo == SAFE || modo == FULL)
	{
		char arreglo[2];
		memcpy(arreglo,&rightVelocity,2);
		char arreglo2[2];
		memcpy(arreglo2,&leftVelocity,2);
		char ins[5] = {(char)0x91,(char)arreglo[1],(char)arreglo[0],(char)arreglo2[1],(char)arreglo2[0]};
		WriteToSerial(descriptorPuerto,ins,5);		
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion DRIVEDIRECT");
}

void Robot::digitalOutputs(unsigned char outputBits)
{
	//Probar funcion 3/7
	if(modo == SAFE || modo == FULL)
	{
		if(outputBits>=0 && outputBits<=7)
		{
			char ins[2] = {(char)0x93,(char)outputBits};
			WriteToSerial(descriptorPuerto,ins,2);
		}
		else
			error(FUERA_RANGO,(char*)"Funcion DIGITALOUTPUTS");
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion DIGITALOUTPUTS");
}

void Robot::stream(vector<unsigned char> packets)
{
	//Probar funcion 4/7
	if(modo==PASSIVE || modo == SAFE || modo == FULL)
	{
		unsigned char* ins = new unsigned char[packets.size()+2];
		ins[0] = (char)0x94;
		ins[1] = (char)packets.size();
		int size = packets.size();
		for(int i = 2;i<size;i++)
		{
			ins[i] = packets[i-2];
		}
		WriteToSerial(descriptorPuerto,(char*)ins,size+2);
		estadoStream = true;
		//Read packets
		 sleep(1);
		 std::thread t(ThreadedReadStream,this,descriptorPuerto,(char *)ins,8);
				t.join();
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion STREAM");
}

void Robot::queryList(vector<unsigned char> packets)
{
	//Probar funcion 5/7
	if(modo==PASSIVE || modo == SAFE || modo == FULL)
	{
		if(packets.size()>=1 && packets.size()<=255)
		{
			unsigned char* ins = new unsigned char[packets.size()+2];
			ins[0] = (char)0x95;
			ins[1] = (char)packets.size();
			int size = packets.size();
			for(int i = 2;i<size;i++)
			{
				ins[i] = packets[i-2];
			}
			WriteToSerial(descriptorPuerto,(char*)ins,size+2);
			//Read packet
			
		}
		else
			error(FUERA_RANGO,(char*)"Funcion QUERYLIST");
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion QUERYLIST");
}

void Robot::pauseResumeStream(bool streamState)
{
	//Probar funcion 6/7
	if(modo == SAFE || modo == FULL)
	{
		char ins[2] = {(char)0x96,(char)streamState};
		WriteToSerial(descriptorPuerto,ins,2);
		estadoStream = streamState;
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion PAUSERESUMESTREAM");
}

void Robot::sendIr(unsigned char byteValue)
{
	//Probar funcion 7/7
	if(modo == SAFE || modo == FULL)
	{
		char ins[2] = {(char)0x97,(char)byteValue};
		WriteToSerial(descriptorPuerto,ins,2);
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion SENDIR");
}

void Robot::script(vector<unsigned char> instrucciones)
{
	if(instrucciones.size()>=1 && instrucciones.size()<=100)
	{
		unsigned char* ins = new unsigned char[instrucciones.size()+2];
		ins[0] = (char)0x98;
		ins[1] = (char)instrucciones.size();
		int size = instrucciones.size();
		for(int i = 2;i<size;i++)
		{
			ins[i] = instrucciones[i-2];
		}
		WriteToSerial(descriptorPuerto,(char*)ins,size+2);
	}
	else
		error(FUERA_RANGO,(char*)"Funcion SCRIPT");
}

void Robot::playScript()
{
	char ins = (char)0x99;
	WriteToSerial(descriptorPuerto,&ins,1);
}

void Robot::showScript()
{
	char ins = (char)0x9A;
	WriteToSerial(descriptorPuerto,&ins,1);
	unsigned char ins2[101];
	usleep(120000);
	ReadFromSerial(descriptorPuerto,(char*)ins2,101);
	int size = (int)ins2[0];
	for(int i=0;i<size;i++)
	{
		cout<<(int)ins2[i]<<" ";
	}
	cout<<endl;
}

void Robot::waitTime(unsigned char time)
{
	char ins[2]{(char)0x9B,(char)time};
	WriteToSerial(descriptorPuerto,ins,2);
}

void Robot::waitDistance(int distance)
{
	if(modo == FULL || modo == SAFE || modo == PASSIVE)
	{
		char arreglo[2];
		memcpy(arreglo,&distance,2);
		char ins[3]{(char)0x9C,(char)arreglo[1],(char)arreglo[0]};
		WriteToSerial(descriptorPuerto,ins,3);
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion WAITDISTANCE");
}

void Robot::waitAngle(int angle)
{
	if(modo == FULL || modo == SAFE || modo == PASSIVE)
	{
		char arreglo[2];
		memcpy(arreglo,&angle,2);
		char ins[3]{(char)0x9D,(char)arreglo[1],(char)arreglo[0]};
		WriteToSerial(descriptorPuerto,ins,3);
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion WAITANGLE");
}

void Robot::waitEvent(signed char event)
{
	if(modo == FULL || modo == SAFE || modo == PASSIVE)
	{
		char ins[2]{(char)0x9E,(char)event};
		WriteToSerial(descriptorPuerto,ins,3);
	}
	else
		error(MODO_INVALIDO_INSTRUCCION,(char*)"Funcion WAITEVENT");
}

void Robot::error(int nError,void *extra)
{
	int* extraAp = (int*)extra;
	char* extraCadena = (char*)extra;
	switch(nError)
	{
		case DEMO_INVALIDO:
			cout<<"Demo invalido(1-9):\t"<<*extraAp<<endl;
		break;
		case BAUD_INVALIDO:
			cout<<"BaudCode invalido(0-11)\t"<<*extraAp<<endl;
		break;
		case MODO_INVALIDO_INSTRUCCION:
			cout<<"La instruccion no se puede ejecutar en este modo:\t"<<extraCadena<<endl;
		break;
		case FUERA_RANGO:
			cout<<"El rango es invalido en "<<extraCadena<<endl;
		break;
		case MODO_INVALIDO:
			cout<<"El modo elegido no existe en la funcion charModo"<<endl;
		break;
		default:
			cout<<"Error desconocido"<<endl;
	}
}

int Robot::getBaudCode(int baudCode)
{
	//La funcion regresa 0 cuando es invalido
	int tmp = 0;
	switch(baudCode)
	{
	case BAUD300:
		tmp = 300;
	break;
	case BAUD600:
		tmp = 600;
	break;
	case BAUD1200:
		tmp = 1200;
	break;
	case BAUD2400:
		tmp = 2400;
	break;
	case BAUD4800:
		tmp = 4800;
	break;
	case BAUD9600:
		tmp = 9600;
	break;
	case BAUD14400:
		tmp = 14400;
	break;
	case BAUD19200:
		tmp = 19200;
	break;
	case BAUD28800:
		tmp = 28800;
	break;
	case BAUD38400:
		tmp = 38400;
	break;
	case BAUD57600:
		tmp = 57600;
	break;
	case BAUD115200:
		tmp = 115200;
	break;
	default:
		error(BAUD_INVALIDO,&baudCode);
	}
	return tmp;
}

char* Robot::charModo(int modoCambio)
{
	//La funcion regresa elcuando es incorrecto el modo
	char * tmp = (char*)"OFF";
	switch(modoCambio)
	{
		case OFF:
			tmp = (char*)"OFF";
		break;
		case FULL:
			tmp = (char*)"FULL";
		break;
		case PASSIVE:
			tmp = (char*)"PASSIVE";
		break;
		case SAFE:
			tmp = (char*)"SAFE";
		break;
		default:
			error(MODO_INVALIDO,(char*)" ");
	}
	return tmp;
}

//-------------------------------------
//Metodos get
//--------------------------------------

bool Robot::getBumpRight()
{
	return bumpRight;
}

bool Robot::getBumpLeft()
{
	return bumpLeft;
}

bool Robot::getWheelDropRight()
{
	return wheelDropRight;
}

bool Robot::getWheelDropLeft()
{
	return wheelDropLeft;
}

bool Robot::getWheelDropCaster()
{
	return wheelDropCaster;
}

bool Robot::getWallSeen()
{
	return wall;
}

bool Robot::getCliffLeft()
{
	return cliffLeft;
}

bool Robot::getCliffFrontLeft()
{
	return cliffFrontLeft;
}

bool Robot::getCliffFrontRight()
{
	return cliffFrontRight;
}

bool Robot::getCliffRight()
{
	return cliffRight;
}

bool Robot::getVirtualWall()
{
	return virtualWall;
}

int Robot::getDistance()
{
	return distance;
}

bool Robot::getLd0()
{
	return ld0;
}

bool Robot::getLd1()
{
	return ld1;
}

bool Robot::getLd2()
{
	return ld2;
}

bool Robot::getRightWheel()
{
	return rightWheel;
}

bool Robot::getLeftWheel()
{
	return leftWheel;
}

bool Robot::getEstadoStream()
{
	return estadoStream;
}

//-------------------------------------
//Metodos paquetes
//--------------------------------------

void Robot::readBumpsAndWheelDrops(char *ins2)
{
	bumpRight = ins2[0] & 0x01;
	bumpLeft = ins2[0] & 0x02;
	wheelDropRight = ins2[0] & 0x04;
	wheelDropLeft = ins2[0] & 0x08;
	wheelDropCaster = ins2[0] & 0x10;
}

void Robot::readWall(char *ins2)
{
	wall = ins2[0];
}

void Robot::readCliffLeft(char* ins2)
{
	cliffLeft = ins2[0];
}

void Robot::readCliffFrontLeft(char*ins2)
{
	cliffFrontLeft = ins2[0];
}

void Robot::readCliffFrontRight(char*ins2)
{
	cliffFrontRight = ins2[0];
}

void Robot::readCliffRight(char*ins2)
{
	cliffRight = ins2[0];
}

void Robot::readVirtualWall(char*ins2)
{
	virtualWall = ins2[0];
}

void Robot::readDistance(char*ins2)
{
	distance = 0;
	char* tmp;
	tmp = (char*) &distance;
	*(tmp) = ins2[1];
	*(tmp+1) = ins2[0];
}

void Robot::readLSDriverAndWheelO(char*ins2)
{	
	ld1 = ins2[0] & 0x01;
	ld0 = ins2[0] & 0x02;
	ld2 = ins2[0] & 0x04;
	rightWheel = ins2[0] & 0x08;
	leftWheel = ins2[0] & 0x10;
}

void Robot::actualizaSensor(unsigned char idPaquete, int sizeInstruccion)
{	
	char ins2[2];
	usleep(120000);
	ReadFromSerial(descriptorPuerto,ins2,sizeInstruccion);
	switch(idPaquete)
	{
		case 7:
			readBumpsAndWheelDrops(ins2);
		break;
		case 8:
			readWall(ins2);
		break;
		case 9:
			readCliffLeft(ins2);
		break;
		case 10:
			readCliffFrontLeft(ins2);
		break;
		case 11:
			readCliffFrontRight(ins2);
		break;
		case 12:
			readCliffRight(ins2);
		break;
		case 13:
			readVirtualWall(ins2);
		break;
		case 14:
			readLSDriverAndWheelO(ins2);
		break;
		case 15:
		
		break;
		case 16:
		
		break;
		case 17:
		
		break;
		case 18:
		
		break;
		case 19:
			readDistance(ins2);
		break;
		case 20:
		
		break;
		case 21:
		
		break;
	}
}
