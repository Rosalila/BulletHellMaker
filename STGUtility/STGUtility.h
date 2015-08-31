#ifndef STGUTILITY_H
#define STGUTILITY_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;
#define PI 3.14159265
#include "../Rosalila/RosalilaInputs/RosalilaInputs.h"

bool getIterateSlowdownFlag();
bool isSlowPressed();
bool isSlowEnabled();
bool isSlowActive();
void slowExtraControl();
void setReceiver(Receiver*receiver_param);
Receiver* getReceiver();
void disableSlow();
void enableSlow();
double getSlowdown();

#endif
