#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <mraa.hpp>
#include <fstream>
#include <vector>
#include <stdio.h>

#include "Master.hpp"

/* Select variable values*/
#define XYPos 0
#define Parameters 1
#define winnerStat 2
#define masterStat 3
#define allfail    4

using boost :: asio :: ip :: address ;
using boost :: asio :: ip :: udp ;
using std::to_string;
using boost::asio::buffer;
using namespace std;	




class Reciever {
private:
		udp::socket &socket;
		Master &master;
public :
	
int xPos1 = -1;
int yPos1 = -1;
int xPos2 = -1;
int yPos2 = -1;
int xPos3 = -1;
int yPos3 = -1;
int select_variable = -1;
int piNumber=0;
int masterPiNum = 0;         
int masterReady =0;
int P1=0;
int P2=0;
int P3=0;
int IamMaster=0;
int life_rec1 =3;
int life_rec2 =3;
int life_rec3 =3;
int pointP1 =0;
int pointP2 =0;
int pointP3 =0;

std::string LABY;
std::string winnerStatus;

Reciever(udp::socket &socket,  Master &master) : socket(socket), master(master) {}
void recieve(OLEDDisplay &lcd);
};
