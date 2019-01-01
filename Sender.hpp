#ifndef _SENDER_HPP
#define _SENDER_HPP

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
using boost :: asio :: ip :: address ;
using boost :: asio :: ip :: udp ;
using std::to_string;
using boost::asio::buffer;
using namespace GFX;
using namespace std;

class Sender
{
	private:
		udp::socket &socket;
		
	public :
	Sender(udp::socket &socket) : socket(socket) {}
		
	void sendInts(int select_variable, int a, int b, int c);
	void sendString(int select_variable, std::string str);
	void sendPoints(int select_variable,std::string player1,int pointP1, std::string player2, int pointP2, std::string player3, int pointP3);
};

#endif
