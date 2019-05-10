#ifndef ROSALILA_UTILITY_H
#define ROSALILA_UTILITY_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
#include <map>

#include <dirent.h>

#ifdef WINDOWS
#include "dirent.h"
#include <direct.h>
#endif
using namespace std;
#define PI 3.14159265

#include "Hitbox.h"

class Hitbox;

class ROSALILA_DLL RosalilaUtility
{
public:
    int random_seed;
    vector<int> non_seeded_random_numbers;
    int current_non_seeded_random_number_index;
    map< string,vector<int>* > checksums;
    string absolute_path;

    void init();
    bool writeLogLine(std::string text);
    bool clearLog();
    std::string toString(int number);
    bool pointIsInRect(int point_x,int point_y,
                  int rect_x,int rect_y,int rect_width,int rect_height);
    bool hitboxCollision(int a_x,int a_y,int a_width,int a_height,float a_angle,
                  int b_x,int b_y,int b_width,int b_height,float b_angle);
    bool hitboxLinesCollision(Line la1,Line la2,Line la3,Line la4,
                  Line lb1,Line lb2,Line lb3,Line lb4);
    Point* lineIntersection(Line l1,Line l2);
    bool segmentIntersection(Line l1,Line l2);
    Point rotateAroundPoint(Point point,Point pivot,float angle);
    Point realRotateAroundPoint(Point point,Point pivot,float angle);
    bool collisionCheck(Hitbox* hb_azul,Hitbox* hb_roja);
    void setRandomSeed(int random_seed);
    int getRandomNumber();
    int getNonSeededRandomNumber();
    double checksumFileA(string file_name);
    double checksumFileB(string file_name);
    double checksumFileC(string file_name);
    bool checkFile(string file_name);
    string getAbsolutePath();
    vector<string> getDirectoryNames(string path);
    vector<string> getFileNames(string path);
};

#endif
