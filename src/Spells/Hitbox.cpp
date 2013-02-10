#include "../include/Spells/Hitbox.h"

Hitbox::Hitbox(int x,int y,int width,int height,float angle)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->angle=angle;
}

int Hitbox::getX()
{
    return x;
}

int Hitbox::getY()
{
    return y;
}

int Hitbox::getWidth()
{
    return width;
}

int Hitbox::getHeight()
{
    return height;
}

float Hitbox::getAngle()
{
    return angle;
}
