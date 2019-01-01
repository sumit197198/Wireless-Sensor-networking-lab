#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "Master.hpp"

int display1[128][128];

/*Select master based on random number. For Pi 1 check the 1st random number
For Pi 2, 2nd random number and so on. Once the value from one Pi is received make flag=1
Find master only when the parameters from all Pis are recieved*/
 void Master :: masterSelect(int piNumber, int P1, int P2, int P3 ){   

 if((piNumber==1) && (flagPi1 == 0) )
	{	 
		P01= P1;
		flagPi1 = 1;
	}
		
	if((piNumber==2) && (flagPi2 == 0) )
	{	 
		P02= P2; 
		flagPi2 = 1;			
	}
		
	if((piNumber==3) && (flagPi3 == 0) )
	{	 
		P03= P3; 
		flagPi3 = 1;
	}
	if (flagPi1 && flagPi2 && flagPi3)
	{
		masterReadySend =1;
		
		if(P01>P02 && P01>P03)
		{
			sender.sendInts(sendmasterStat,1,masterReadySend,5);
		}
		
		if(P02>P01 && P02>P03)
		{
			sender.sendInts(sendmasterStat,2,masterReadySend,5);
		}

		if(P03>P01 && P03>P02)
		{
		  sender.sendInts(sendmasterStat,3,masterReadySend,5);
		}
	}
 }
 void getInput1(){
	int i,j;
	i=0;
	string line;
	ifstream myfile("Array.txt");
	if (myfile.is_open()){
		for (i=0; i<128; i++){
			getline (myfile, line);
			for (j=0; j<128; j++){
				display1[j][i]= line[j];
			}
		}
	myfile.close();
	}
	else cout<< "Unable to open file \n";
}
 
/* If there is a winner Send the information to other Pis*/
void Master :: displayWinner(int xPos1, int yPos1, int xPos2, int yPos2, int xPos3, int yPos3, int life_rec1, int life_rec2,int life_rec3,int pointP1,int pointP2,int pointP3) 
{
		getInput1();
/* Check if any Pi has reached the centre, If so declare winner*/
		if (display1[xPos1][yPos1]== '1' ) 
		{
			sender.sendString(sendwinnerStat,"PLAYER 1 WINS!!");	
		}
		if (display1[xPos2][yPos2]== '1' ) 
		{
			sender.sendString(sendwinnerStat,"PLAYER 2 WINS!!");
		}
		if (display1[xPos3][yPos3]== '1')  
		{
			sender.sendString(sendwinnerStat,"PLAYER 3 WINS!!");	
		}
/*Check if all the players have failed. Then display the points of each player*/
		if(life_rec1 < 0 && life_rec2 < 0 && life_rec3 < 0)
		{
			sender.sendPoints(sendallfailStat, "PLAYER1", pointP1,"PLAYER2",pointP2,"PLAYER3",pointP3);
					
		}

}

