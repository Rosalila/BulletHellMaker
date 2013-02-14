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
    Hitbox();
    Hitbox(int x,int y,int width,int height,float angle);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    Hitbox getPlacedHitbox(Point p,float a);
    float getAngle();
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);
    void setAngle(float angle);
    void setValues(int x,int y, int width, int height,float angle);
    bool collides(Hitbox hitbox_param);
};

#endif
