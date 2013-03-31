#include "Hitbox.h"

Hitbox::Hitbox(int x,int y,int width,int height,float angle)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->angle=angle;
}

Hitbox::Hitbox()
{
    this->x=0;
    this->y=0;
    this->width=0;
    this->height=0;
    this->angle=0;
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

void Hitbox::setX(int x)
{
    this->x=x;
}
void Hitbox::setY(int y)
{
    this->y=y;
}
void Hitbox::setWidth(int width)
{
    this->width=width;
}
void Hitbox::setHeight(int height)
{
    this->height=height;
}
void Hitbox::setAngle(float angle)
{
    this->angle=angle;
}

void Hitbox::setValues(int x,int y, int width, int height,float angle)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->angle=angle;
}

bool Hitbox::collides(Hitbox hitbox_param)
{
    return hitboxCollision(this->getX(),this->getY(),this->getWidth(),this->getHeight(),this->getAngle(),
                    hitbox_param.getX(),hitbox_param.getY(),hitbox_param.getWidth(),hitbox_param.getHeight(),hitbox_param.getAngle())
        || hitboxCollision(hitbox_param.getX(),hitbox_param.getY(),hitbox_param.getWidth(),hitbox_param.getHeight(),hitbox_param.getAngle(),
                    this->getX(),this->getY(),this->getWidth(),this->getHeight(),this->getAngle());
}

bool Hitbox::collides(Hitbox hitbox_param,int hitbox_x,int hitbox_y,int hitbox_angle)
{
    hitbox_param.setX(hitbox_param.getX()+hitbox_x);
    hitbox_param.setY(hitbox_param.getY()+hitbox_y);
    hitbox_param.setAngle(hitbox_param.getAngle()+hitbox_angle);

    return hitboxCollision(this->getX(),this->getY(),this->getWidth(),this->getHeight(),this->getAngle(),
                    hitbox_param.getX(),hitbox_param.getY(),hitbox_param.getWidth(),hitbox_param.getHeight(),hitbox_param.getAngle())
        || hitboxCollision(hitbox_param.getX(),hitbox_param.getY(),hitbox_param.getWidth(),hitbox_param.getHeight(),hitbox_param.getAngle(),
                    this->getX(),this->getY(),this->getWidth(),this->getHeight(),this->getAngle());
}

Hitbox Hitbox::getPlacedHitbox(Point p,float a)
{
    Hitbox hitbox = *this;
    Point rotated=rotateAroundPoint(Point(hitbox.getX(),hitbox.getY()),
                                    Point(p.x,p.y),
                                    a+hitbox.getAngle());
    hitbox.setX(rotated.x);
    hitbox.setY(rotated.y);
    hitbox.setAngle(a+hitbox.getAngle());
    return hitbox;
}
