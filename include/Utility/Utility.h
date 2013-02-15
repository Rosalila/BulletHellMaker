#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;
#define PI 3.14159265

class Point
{
public:
    int x;
    int y;
    Point()
    {
        this->x=0;
        this->y=0;
    }
    Point(int x,int y)
    {
        this->x=x;
        this->y=y;
    }
};

class Line
{
public:
    Point p1;
    Point p2;
    Line()
    {
    }
    Line(Point p1,Point p2)
    {
        this->p1=p1;
        this->p2=p2;
    }
};

bool writeLogLine(std::string text);
bool clearLog();
std::string toString(int number);
bool pointIsInRect(int point_x,int point_y,
              int rect_x,int rect_y,int rect_width,int rect_height);
bool hitboxCollision(int a_x,int a_y,int a_width,int a_height,float a_angle,
              int b_x,int b_y,int b_width,int b_height,float b_angle);
Point* lineIntersection(Line l1,Line l2);
bool segmentIntersection(Line l1,Line l2);
Point rotateAroundPoint(Point point,Point pivot,float angle);

#endif
