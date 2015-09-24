#include"Robot.h"

vector<Nota> construyeCancion()
{
	vector<Nota> tmp;
	Nota tmpNota1(62,12);
	tmp.push_back(tmpNota1);
	Nota tmpNota2(66,12);
	tmp.push_back(tmpNota2);
	Nota tmpNota3(69,12);
	tmp.push_back(tmpNota3);
	return tmp;
}

vector<unsigned char> construyeScript()
{
	vector<unsigned char> tmp;
	//137 1 44 128 0 156 1 144 137 0 0 0 0
	tmp.push_back(137);
	tmp.push_back(1);
	tmp.push_back(44);
	tmp.push_back(128);
	tmp.push_back(0);
	tmp.push_back(156);
	tmp.push_back(1);
	tmp.push_back(144);
	tmp.push_back(137);
	tmp.push_back(0);
	tmp.push_back(0);
	tmp.push_back(0);
	tmp.push_back(0);
	return tmp;
}

vector<unsigned char> construyeStream()
{
	vector<unsigned char> tmp;
	tmp.push_back(29);
	tmp.push_back(13);
	return tmp;
}

int main()
{
	Robot robot("/dev/ttyUSB0");
	robot.safe();
	sleep(1);
	//robot.baud(12);
	//robot.demo(8);
	//robot.full();
	//robot.cover();
	//robot.coverAndDock();
	//robot.spot();
	//robot.leds((char)0x02,(char)0x00,(char)0x00);
	//robot.waitTime(30);
	//robot.song((unsigned char)0,construyeCancion());
	//robot.playSong(0);
	//robot.script(construyeScript());
	//robot.playScript();
	//robot.drive(15,-15);
	//robot.driveDirect(20,20);
	//robot.waitDistance(-100);
	//robot.waitAngle(-45);
	robot.sensors(19);
	//robot.showScript();
	//robot.waitEvent(12);
	//robot.driveDirect(0,0);
	//robot.stream(construyeStream());
	return 0;
}
