#ifndef STGUTILITY_H
#define STGUTILITY_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;
#define PI 3.14159265
#include "RosalilaInputs/RosalilaInputs.h"



bool getIterateSlowdownFlag();
bool isSlowPressed();
void slowExtraControl();
void setReceiver(Receiver*receiver_param);
Receiver* getReceiver();

#endif
