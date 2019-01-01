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
#include "Marble.hpp"
using boost :: asio :: ip :: address ;
using boost :: asio :: ip :: udp ;
using std::to_string;
using boost::asio::buffer;
using namespace GFX;
using namespace std;


/*Send parameters and coordinates and master ready status */
/* Message structure --> LABY+Pi Number+Select variable+ (Xpos + Ypos + Life) or (Rand No1 + Rand No2 +Rand No3) or (master pi number+ masterready status + buffer value)*/
void Sender::sendInts(int select_variable, int x, int y, int z)

{
	udp::endpoint target(address::from_string("192.168.0.255"),7788);	
	boost::system::error_code err;	
	string str = string("LABY:") + to_string(myPiNum) + ":"+to_string(select_variable) + ":"+ to_string((long)x ) + ":" + to_string((long) y) +":" + to_string((long) z);
	socket.send_to(buffer(str), target, 0, err);
	
}

/*Send winner status
Message structure --> LABY+ pi Number + select variable + who is winner*/

void Sender::sendString(int select_variable, string winnerStatus)
{
	udp::endpoint target(address::from_string("192.168.0.255"),7788);	
	boost::system::error_code err;	
	string str = string("LABY:") + to_string(myPiNum) + ":"+to_string(select_variable) + ":"+ winnerStatus;
	socket.send_to(buffer(str), target, 0, err);
		
}

/* Send the points if all the Pis lost their lifes
Message structure --> LABY+ Pi number + select variable + Player1 + Points of Player 1 + Player 2 + Points of Player 2 + Player 3 + Points of Player 3*/
void Sender::sendPoints(int select_variable,string player1,int pointP1, string player2, int pointP2, string player3, int pointP3 )
{
	udp::endpoint target(address::from_string("192.168.0.255"),7788);	
	boost::system::error_code err;	
	string str = string("LABY:") + to_string(myPiNum) + ":"+ to_string(select_variable) + ":"+ player1 + ":"+to_string(pointP1) + ":"+ player2 + ":"+to_string(pointP2) +":"+ player3 + ":"+to_string(pointP3)  ;
	socket.send_to(buffer(str), target, 0, err);
		
}





