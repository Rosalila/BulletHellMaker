#ifndef HITBOX_H
#define HITBOX_H

//Choose your build platform by defining it:
#ifdef __linux__
#define LINUX
#endif

#ifdef __APPLE__
#define OSX
#endif
#ifdef __MACH__
#define OSX
#endif

#ifdef _WIN32
#define WINDOWS
#endif
#ifdef _WIN64
#define WINDOWS
#endif

#ifdef ROSALILA_STATICLIB
#  define ROSALILA_DLL
#elif defined(WINDOWS)
#  ifdef ROSALILA_DYNAMICLIB
#    define ROSALILA_DLL  __declspec(dllexport)
#  else
#    define ROSALILA_DLL  __declspec(dllimport)
#  endif
#else
#  define ROSALILA_DLL
#endif


#include "Line.h"
#include "../RosalilaGraphics/RosalilaGraphics.h"
#include "../RosalilaSound/RosalilaSound.h"
#include "../RosalilaInputs/RosalilaInputs.h"


class ROSALILA_DLL Hitbox
{
public:
    int x,y;
    int width,height;
    float angle;
    Line line1;
    Line line2;
    Line line3;
    Line line4;
public:
    Hitbox();
    Hitbox(int x,int y,int width,int height,float angle);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    Hitbox getPlacedHitbox(double x,double y,float a);
    Hitbox getPlacedHitbox(double x,double y);
    float getAngle();
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);
    void setAngle(float angle);
    void setValues(int x,int y, int width, int height,float angle);
    bool collides(Hitbox hitbox_param);
    bool collides(Hitbox hitbox_param,int hitbox_x,int hitbox_y,int hitbox_angle);
    void setLines();
    Hitbox getFlippedHitbox();
};

#endif
