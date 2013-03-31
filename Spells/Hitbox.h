#ifndef HITBOX_H
#define HITBOX_H

#include "TinyXml/tinyxml.h"
#include "RosalilaGraphics/RosalilaGraphics.h"
#include "RosalilaSound/RosalilaSound.h"
#include "RosalilaInputs/RosalilaInputs.h"

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
    bool collides(Hitbox hitbox_param,int hitbox_x,int hitbox_y,int hitbox_angle);
};

#endif
