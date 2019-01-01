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
#include <chrono>
#include "OLEDDisplay.h"
#include <vector>
#include <stdio.h>
#include "Reciever.hpp"
#include "Marble.hpp"
using boost :: asio :: ip :: address ;
using boost :: asio :: ip :: udp ;
using std::to_string;
using boost::asio::buffer;
using namespace GFX;
using namespace std;
using std::chrono::steady_clock;
 
 std::vector<std::string> split(std::string str, char delimiter) {
  std::vector<std::string> aux;
  std::stringstream ss(str); // Turn the string into a stream.
  std::string tok;
  
  while(getline(ss, tok, delimiter)) {
    aux.push_back(tok);
  }
  
  return aux;
}

void Reciever::recieve(OLEDDisplay &lcd_win) 
{
	udp::endpoint sender;
	boost::array<char, 200> buf;	
	boost::system::error_code err;

	
	
	while (1) {

		size_t len = socket.receive_from(buffer(buf), sender, 0, err);
		if (err) {
			if (err != boost::asio::error::would_block) {
				printf("Error\n");
			}
			break;
		}
		std::string message(buf.c_array(), len); 	
		std::vector<std::string> parsed = split(message, ':');
		std:: string LABY = parsed[0];
		if (LABY == "LABY"){                                //Check for the first string, if not LABY discard all
			piNumber= std::stoi (parsed[1]);
			
/*If the message from first Pi is recieved*/
			if (piNumber == 1){
				select_variable = std::stoi (parsed[2]);
/*If the message is the random number form the Pi*/
				if (select_variable == Parameters){        
					 P1 = std::stoi(parsed[3]);
					 P2 = std::stoi(parsed[4]);
					 P3 = std::stoi(parsed[5]);
					master.masterSelect(piNumber,P1,P2,P3);
				}
/*If the message is XY Position form the Pi*/
				else if (select_variable == XYPos){               //coordinates
					xPos1 = std::stoi(parsed[3]);
					yPos1 = std::stoi(parsed[4]);
					life_rec1 = std::stoi(parsed[5]);
					if (life_rec1 ==0){
						pointP1 = 126 - yPos1;
					}
				}
/*Declaration of winner from master*/
				else if (select_variable == winnerStat){
					std:: string winnerStatus = parsed[3];    //winner
					lcd_win.clearScreen();
					while(1)
					{
						lcd_win.setTextColor(COLOR_WHITE);
						lcd_win.drawRect(5, 10, 110, 55);
						lcd_win.setCursor(15, 30);
						lcd_win.print(parsed[3]);
						lcd_win.setCursor(15, 110);
						lcd_win.print("START NEW GAME!!");
						lcd_win.flush();
					}
				}
/*If the master is selected, display who is master*/
				else if (select_variable == masterStat && masterReady != 1){
					masterPiNum = std::stoi(parsed[3]);// if I am the master
					masterReady = std::stoi(parsed[4]);
					if(masterPiNum == myPiNum){
						IamMaster = 1;
					}
					lcd_win.setTextColor(COLOR_WHITE);
					lcd_win.setCursor(50, 30);
					lcd_win.print("PLAYER");
					lcd_win.setCursor(60,40 );
					lcd_win.print(parsed[3]);
					lcd_win.setCursor(50, 50);
					lcd_win.print("MASTER");
					lcd_win.flush();
					std::this_thread::sleep_for(std::chrono::seconds(2));
					lcd_win.clearScreen();
				}
/*If all failed to reach destination, then display the points recieved from master*/
				else if (select_variable == allfail){	
						lcd_win.clearScreen();

				while(1)
				{
					lcd_win.setTextColor(COLOR_WHITE);
					lcd_win.setCursor(36, 10);
					lcd_win.print("SCOREBOARD");
					lcd_win.setCursor(34, 15);
					lcd_win.print("-----------");
					lcd_win.setCursor(36, 30);
					lcd_win.print(parsed[3]);
					lcd_win.setCursor(90, 30);
					lcd_win.print(parsed[4]);
					lcd_win.setCursor(36, 50);
					lcd_win.print(parsed[5]);
					lcd_win.setCursor(90, 50);
					lcd_win.print(parsed[6]);
					lcd_win.setCursor(36, 70);
					lcd_win.print(parsed[7]);
					lcd_win.setCursor(90, 70);
					lcd_win.print(parsed[8]);
					lcd_win.setCursor(20, 110);
					lcd_win.print("START NEW GAME!!");
					lcd_win.flush();
				}
			}
					
		}
/*Repeat for all Pis*/ 
			//If the message from second Pi is recieved
			if (piNumber == 2){
				select_variable = std::stoi (parsed[2]);
				if (select_variable == Parameters){
					P1 = std::stoi(parsed[3]);
					P2 = std::stoi(parsed[4]);
					P3 = std::stoi(parsed[5]);
					master.masterSelect(piNumber,P1,P2,P3);
				}
				else if (select_variable == XYPos){
					xPos2 = std::stoi(parsed[3]);
					yPos2 = std::stoi(parsed[4]);
					life_rec2 = std::stoi(parsed[5]);
					if (life_rec2 ==0){
						pointP2 = 126 - yPos2;
					}
				}
				else if (select_variable == winnerStat){
					std:: string status = parsed[3];
					lcd_win.clearScreen();
					while(1)
					{
						lcd_win.setTextColor(COLOR_WHITE);
						lcd_win.drawRect(5, 10, 110, 55);
						lcd_win.setCursor(15, 30);
						lcd_win.print(parsed[3]);
						lcd_win.setCursor(15, 110);
						lcd_win.print("START NEW GAME!!");
						lcd_win.flush();
					}
				}
				else if (select_variable == masterStat && masterReady != 1){
					masterPiNum = std::stoi(parsed[3]);
					masterReady = std::stoi(parsed[4]);
					if(masterPiNum == myPiNum){
						IamMaster = 1;
					}
					lcd_win.setTextColor(COLOR_WHITE);
					lcd_win.setCursor(50, 30);
					lcd_win.print("PLAYER");
					lcd_win.setCursor(60,40 );
					lcd_win.print(parsed[3]);
					lcd_win.setCursor(50, 50);
					lcd_win.print("MASTER");
					lcd_win.flush();
					std::this_thread::sleep_for(std::chrono::seconds(2));
					lcd_win.clearScreen();
					
				}
				else if (select_variable == allfail){	
				lcd_win.clearScreen();
				while(1)
						{
						lcd_win.setTextColor(COLOR_WHITE);
						lcd_win.setCursor(36, 10);
						lcd_win.print("SCOREBOARD");
						lcd_win.setCursor(34, 15);
						lcd_win.print("-----------");
						lcd_win.setCursor(36, 30);
						lcd_win.print(parsed[3]);
						lcd_win.setCursor(90, 30);
						lcd_win.print(parsed[4]);
						lcd_win.setCursor(36, 50);
						lcd_win.print(parsed[5]);
						lcd_win.setCursor(90, 50);
						lcd_win.print(parsed[6]);
						lcd_win.setCursor(36, 70);
						lcd_win.print(parsed[7]);
						lcd_win.setCursor(90, 70);
						lcd_win.print(parsed[8]);
						lcd_win.setCursor(20, 110);
						lcd_win.print("START NEW GAME!!");
						lcd_win.flush();
						}
						
				}
					
			}
			//If the message from third Pi is recieved
			if (piNumber == 3){
				select_variable = std::stoi (parsed[2]);
				if (select_variable == Parameters){
					P1 = std::stoi(parsed[3]);
					P2 = std::stoi(parsed[4]);
					P3 = std::stoi(parsed[5]);
					master.masterSelect(piNumber,P1,P2,P3);
				}
				else if (select_variable == XYPos){
					xPos3 = std::stoi(parsed[3]);
					yPos3 = std::stoi(parsed[4]);
					life_rec3 = std::stoi(parsed[5]);
					if (life_rec3 ==0){
						pointP3 = 126 - yPos3;
					}
				}
				else if (select_variable == winnerStat){
					std:: string status = parsed[3];
					lcd_win.clearScreen();
					while(1)
					{
						lcd_win.drawRect(5, 10, 120, 55);
						lcd_win.setCursor(15, 30);
						lcd_win.print(parsed[3]);
						lcd_win.setCursor(15, 110);
						lcd_win.print("START NEW GAME!!");
						lcd_win.flush();
					}
				}
				else if (select_variable == masterStat && masterReady != 1){
					masterPiNum = std::stoi(parsed[3]);
					masterReady = std::stoi(parsed[4]);
					if(masterPiNum == myPiNum){
						IamMaster = 1;
					}
					lcd_win.setTextColor(COLOR_WHITE);
					lcd_win.setCursor(50, 30);
					lcd_win.print("PLAYER");
					lcd_win.setCursor(60,40 );
					lcd_win.print(parsed[3]);
					lcd_win.setCursor(50, 50);
					lcd_win.print("MASTER");
					lcd_win.flush();
					std::this_thread::sleep_for(std::chrono::seconds(1));
					lcd_win.clearScreen();
				}
				else if (select_variable == allfail){
					lcd_win.clearScreen();	
					while(1)
					{
					lcd_win.setTextColor(COLOR_WHITE);
					lcd_win.setCursor(36, 10);
					lcd_win.print("SCOREBOARD");
					lcd_win.setCursor(34, 15);
					lcd_win.print("-----------");
					lcd_win.setCursor(36, 30);
					lcd_win.print(parsed[3]);
					lcd_win.setCursor(90, 30);
					lcd_win.print(parsed[4]);
					lcd_win.setCursor(36, 50);
					lcd_win.print(parsed[5]);
					lcd_win.setCursor(90, 50);
					lcd_win.print(parsed[6]);
					lcd_win.setCursor(36, 70);
					lcd_win.print(parsed[7]);
					lcd_win.setCursor(90, 70);
					lcd_win.print(parsed[8]);
					lcd_win.setCursor(20, 110);
					lcd_win.print("START NEW GAME!!");
					lcd_win.flush();
					}
				}	
			}
		}		
	}	
}
		

