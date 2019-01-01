#ifndef _MASTER_HPP
#define _MASTER_HPP

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
#include "OLEDDisplay.h"
#include <vector>
#include <stdio.h>
#include "Sender.hpp"
#include <iostream>
#include <random>
#include "Marble.hpp"

/* Select variable values*/
#define sendwinnerStat 2
#define sendmasterStat 3
#define sendallfailStat 4


using boost :: asio :: ip :: address ;
using boost :: asio :: ip :: udp ;
using std::to_string;
using boost::asio::buffer;
using namespace GFX;
using namespace std;


class Master{
private:
Sender &sender;

public:
bool decideMaster=false,flagPi1=false,flagPi2=false,flagPi3=false;
int P01,P02,P03;
int masterReadySend = 0;

Master(Sender &sender) : sender(sender) {}

void masterSelect(int piNumber, int P1, int P2, int P3 );
void displayWinner(int xPos1, int yPos1, int xPos2, int yPos2, int xPos3, int yPos3, int life_rec1, int life_rec2,int life_rec3, int pointp1,int pointP2, int pointP3);

};

#endif
