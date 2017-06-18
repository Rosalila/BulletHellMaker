#ifndef STGUTILITY_H
#define STGUTILITY_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;
#define PI 3.14159265
#include "../../Rosalila/Rosalila.h"

void initShooterUtility();

bool getIterateSlowdownFlag();
bool isSlowPressed();
bool isSlowEnabled();
bool isSlowActive();
void slowExtraControl();
void disableSlow();
void enableSlow();
double getSlowdown();
bool getGameOver();
void setGameOver(bool game_over_param);
bool getPlayerWon();
void setPlayerWon(bool player_won_param);
bool getIsFirstWin();
void setIsFirstWin(bool is_first_win_param);

Image* getLoadingImage();
Image* getErrorImage();
Image* getSuccessImage();

int getNotificationDuration();

vector<int>* getColorPaletteR();
vector<int>* getColorPaletteG();
vector<int>* getColorPaletteB();

#endif
