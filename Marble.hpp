#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mraa.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "OLEDDisplay.h"

/* Select variable values*/
#define sendXYPos 0
#define sendParameters 1

using std::chrono::steady_clock;
using namespace GFX;
using namespace std;

void get_parameters();
void getInput();

extern int myPiNum;




	