#include "Marble.hpp"
#include "Sender.hpp"
#include "Master.hpp"
#include "Reciever.hpp"
#include<ctime>

using std::to_string;


int myPiNum = 1;

int display [128][128];

/* Mid point of the screen, Needed for rotating the marble*/
float xmid = 63;
float ymid = 63;

/*Initial position*/
float x0 = 8; 

float xdiff1,ydiff1,xdiff2,ydiff2,xnew10,ynew10,xnew20,ynew20;;
float xnew0 = 0;
float ynew0 = 0;

/*Initial speed and acceleration*/ 
float speedx = 0;
float speedy = 0;
float speedx1 = 0;
float speedy1 = 0;
float accx = 0;
float accy = 0;

/* dt and frictional factor*/
float dt = 0.2;
float friction = 0.98;

/* Initialisation of random number*/
int random_no1 = 0;
int random_no2 = 0;
int random_no3 = 0;

/*flags - reach final point, end of lives*/
bool reach_dest =false;
bool end_life =false;

/*Function for generating random numbers*/
void get_parameters()
{
	srand((unsigned)time(0));
	 random_no1 = rand();
	 random_no2 = rand();
	 random_no3 = rand();
	 random_no1 = random_no1/100000;
	 random_no2 = random_no2/100000;
	 random_no3 = random_no3/100000;
}

/* Function for reading the pixel value from Array.txt and storing in a 128x128 matrix*/
void getInput(){
	int i,j;
	i=0;
	string line;
	ifstream myfile("Array.txt");
	if (myfile.is_open()){
		for (i=0; i<128; i++){
			getline (myfile, line);
			for (j=0; j<128; j++){
				display[j][i]= line[j];
			}
		}
	myfile.close();
	}
	else cout<< "Unable to open file \n";
}

/* Draw the playground*/
void lcd_display(OLEDDisplay &lcd){
	lcd.clearScreen();
	lcd.enable();
	int k,l;
	for (l=0; l<128; l++){
		for (k=0; k<128; k++){
			if (display[l][k] >= '1')
			{
				lcd.drawPixel(l,k);
			}
		}
	}
			
	lcd.setCursor(36, 12);
	lcd.setTextColor(COLOR_WHITE);
	lcd.print("LABYRINTH");
	lcd.setCursor(38, 23);
	lcd.print("PLAYER: ");
	lcd.setCursor(78, 23);
	lcd.print(to_string(myPiNum));
}

/* Movement of the marble*/
void playLaby(Sender &sender , Reciever &reciever,Master & master, udp::socket &socket, OLEDDisplay &lcd) {
	lcd.clearScreen();
/*Initial display*/
	lcd.setCursor(20, 25);
	lcd.print("GAME STARTS IN ");
	lcd.flush();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	lcd.clearScreen();
	lcd.setCursor(63, 25);
	lcd.print("3 ");
	lcd.flush();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	lcd.clearScreen();
	lcd.setCursor(63, 25);
	lcd.print("2 ");
	lcd.flush();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	lcd.clearScreen();
	lcd.setCursor(63, 25);
	lcd.print("1 ");
	lcd.flush();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	lcd.clearScreen();
	lcd.setCursor(60, 25);
	lcd.print("GO");
	lcd.flush();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
	float y0 = 124;

	const int radius = 2;
	const int minX = radius;
	const int maxX = lcd.getWidth() - radius;
	const int maxY = lcd.getHeight() - radius;

/*Initialising I2c and the acceleration sensor registers*/ 
	mraa::I2c i2c(0);
	i2c.address(0x68);
	i2c.writeReg(0x6B, 0x80);
	usleep(100*1000);
	usleep(100000);
	i2c.writeReg(0x6B, 0x00);
	
/* Repeat for ever*/
	while (true) {
		lcd.clearScreen();
		lcd_display(lcd);

/* Read raw accelerometer values*/
		uint8_t acc_raw[6];
		int len =i2c.readBytesReg(0x3B, acc_raw,6);
		if(len <0)
		{
			printf("Error!|n");
		}
		else{
			int16_t ax_raw = (int16_t) ((acc_raw[0]<<8) + acc_raw[1]);
			int16_t ay_raw = (int16_t) ((acc_raw[2]<<8) + acc_raw[3]);
			int16_t az_raw = (int16_t) ((acc_raw[4]<<8) + acc_raw[5]);
		
			accx = ax_raw / -16384.0f;
			accy = ay_raw / 16384.0f;
			float az = az_raw / 16384.0f;
		}
		speedx1 = (speedx + (accx*dt)) * friction;
		speedy1 = (speedy + (accy*dt)) * friction;

		xnew0  = x0 + (speedx1*dt);
		ynew0  = y0 + (speedy1*dt);

/* limit the movement of the marble to just one pixel at a time*/
		int diff = 0;
		if((std::abs(xnew0 - x0)) > 1)
		{
		
		 	diff = xnew0 - x0;
			if( diff > 0)
				xnew0 = x0 + 1;
			else
				xnew0 = x0 - 1;
		}
		if((std::abs(ynew0 - y0)) > 1)
		{
			diff = ynew0 - y0;
			if (diff > 0)
				ynew0 = y0 + 1;
			else
				ynew0 = y0 - 1;
		}	
/* Check for slanting boundary at the right, make horizontal speed to zero*/
		if(display[int(xnew0)][int(ynew0)] == '3')
		{
			if(xnew0 < x0)
			{
				xnew0 = x0;
				speedx1 = 0;
			}
			ynew0 = y0;
		}
/* Check for slanting boundary at the left, make horizontal speed to zero*/
		if(display[int(xnew0)][int(ynew0)] == '4')
		{
			if(xnew0 > x0)
			{
				xnew0 = x0;
				speedx1 = 0;
			}
			ynew0 = y0;
		}
/* If new position is at the centre then display finished, wait for master to declare winner*/		
		if(reach_dest){
			lcd.setCursor(38, 33);
			lcd.setTextColor(COLOR_WHITE);
			lcd.print("FINISHED");
			xnew0=x0;
			ynew0=y0;
		}
/* If no lifes are left display "GAME OVER" */
		if(end_life){
			lcd.setCursor(36, 33);
			lcd.setTextColor(COLOR_WHITE);
			lcd.print("GAME OVER");
			xnew0=x0;
			ynew0=y0;
		}
/* Calculation of life */
		static int life =2;
		if(display[int(xnew0)][int(ynew0)] == '7')
			{	
				if(life ==0)
					end_life =1;
				else{
					lcd.setCursor(36, 30);
					lcd.setTextColor(COLOR_WHITE);
					lcd.print("LIFE OVER");
					xnew0 =4;
					ynew0 =125;
					}
				life--;
			}
/*If reached centre, then make flag reach_dest to 1*/
		if(display[int(xnew0)][int(ynew0)] == '1')
			{
				reach_dest =1;
			} 
/*If boundary stay at original position*/
		if (display[int(xnew0)][int(ynew0)]=='5' )
		{
			xnew0 = x0;
			ynew0 = y0;
		}
/*If boundary stay at original position*/
		if (display[int(xnew0)][int(ynew0)]=='6')
		{
			xnew0 = x0;
			ynew0 = y0;
		}

/*limit marble inside playground*/
        if (xnew0 < 2) { xnew0=2; speedx1=0;}
        if (xnew0 > 125) { xnew0=125; speedx1=0;}
        if (ynew0 < 2) { ynew0=2; speedy1=0;}
        if (ynew0 > 125) { ynew0=125; speedy1=0;}
/*Call reciever to get values from other Pis*/
		reciever.recieve(lcd); 
	
/*If master, check if any Pi has reached destination or if all of them lost*/
		if (reciever.IamMaster == 1)
		{
			master.displayWinner(reciever.xPos1,reciever.yPos1,reciever.xPos2,reciever.yPos2,reciever.xPos3,reciever.yPos3,reciever.life_rec1,reciever.life_rec2,reciever.life_rec3,reciever.pointP1,reciever.pointP2,reciever.pointP3);
		}
	
/*Rotate the X and Y position for other pis*/
		if (myPiNum ==1){	
			xdiff1 = reciever.xPos2 - xmid;
			ydiff1 = reciever.yPos2 - ymid;
			xdiff2 = reciever.xPos3 - xmid;
			ydiff2 = reciever.yPos3 - ymid;
			
			ynew10 = xmid + xdiff1;
			xnew10 = ymid - ydiff1 +1;

			ynew20 = xmid -xdiff2 +1;
			xnew20 = ymid +ydiff2;
		}
		else if (myPiNum ==2){
			xdiff1 = reciever.xPos1 - xmid;
			ydiff1 = reciever.yPos1 - ymid;
			xdiff2 = reciever.xPos3 - xmid;
			ydiff2 = reciever.yPos3 - ymid;
			
			ynew10 = xmid + xdiff1;
			xnew10 = ymid - ydiff1 +1;

			ynew20 = xmid -xdiff2 +1;
			xnew20 = ymid +ydiff2;
		}
		else if (myPiNum ==3){
			xdiff1 = reciever.xPos1 - xmid;
			ydiff1 = reciever.yPos1 - ymid;
			xdiff2 = reciever.xPos2 - xmid;
			ydiff2 = reciever.yPos2 - ymid;
			
			ynew10 = xmid + xdiff1;
			xnew10 = ymid - ydiff1 +1;

			ynew20 = xmid -xdiff2 +1;
			xnew20 = ymid +ydiff2;
		}

/*Set the speed to new speed*/	
		speedx = speedx1;
		speedy = speedy1;
		x0 = xnew0;
		y0 = ynew0;

/*Move marble of all Pis*/		
		lcd.drawCircle(xnew0,ynew0,radius);
		lcd.drawCircle(xnew10,ynew10,radius);
		lcd.drawCircle(xnew20,ynew20,radius);

/*Display lifes*/		
		int i;
		for(i = 0; i <= life; i++)
		{
			lcd.fillCircle(55+(i*7), 5, radius);
		}		

/*Send the value of the new position to other pis*/
		xnew0 = int(xnew0);
		ynew0 = int(ynew0);
			
		sender.sendInts(sendXYPos,xnew0, ynew0,life);  // Send my current position
		
		lcd.flush();
	}
}

int main(int argc, char **argv) {
	printf("Starting\n");
	
	myPiNum = 1;
	if (argc > 1) {
		myPiNum = std::stoi(argv[1]);
	}
	
	OLEDDisplay lcd(128, 128);
	lcd.enable();
	lcd.setBgColor(COLOR_BLACK);
	lcd.setDrawColor(COLOR_WHITE);
	lcd.setTextColor(COLOR_WHITE);
	lcd.clearScreen();
	lcd.flush();
	getInput();
	
	boost::asio::io_service io_service;
	udp::socket socket(io_service);
	socket.open(udp::v4());
	socket.set_option(udp::socket::reuse_address(true));
	socket.set_option(udp::socket::broadcast(true));
	socket.bind(udp::endpoint(udp::v4(), 7788));
	socket.non_blocking(true); 

	Sender sender(socket);
	Master master(sender);
	Reciever reciever(socket, master);
	get_parameters();
	
/* When master is not ready, send parameters*/
	while(!reciever.masterReady){
		lcd.setCursor(15, 15);
		lcd.print("DECIDING MASTER... ");
		lcd.flush();
		sender.sendInts(sendParameters,random_no1,random_no2,random_no3);
		reciever.recieve(lcd);
		master.masterSelect(reciever.piNumber, reciever.P1, reciever.P2, reciever.P3 );
	}	
	playLaby(sender,reciever, master,socket,lcd);
	return 0;
}
