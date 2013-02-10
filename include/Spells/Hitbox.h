#ifndef HITBOX_H
#define HITBOX_H

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Input/Input.h"

class Hitbox
{
    int x,y;
    int width,height;
    float angle;
public:
    Hitbox(int x,int y,int width,int height,float angle);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    float getAngle();
};

#endif
